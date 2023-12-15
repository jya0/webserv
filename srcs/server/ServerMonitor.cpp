/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/15 05:19:24 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ServerMonitor.hpp"

ServerMonitor::ServerMonitor(const std::vector<ServerConfig> &configsREF) : _sockets(configsREF.size())
{
	WebServer *server;

	for (std::vector<ServerConfig>::const_iterator itr = configsREF.begin(); itr != configsREF.end(); itr++)
	{
		try
		{
			server = new WebServer(*itr);
			_servers.insert(std::make_pair(server->getConnection().getPassiveSocket(), server));
		}
		catch (ServerSocket::SocketIOError &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	_sockets = PollManager(_servers.size());
	http::CGIhandler::setPollManager(_sockets);
	_cgiScripts.clear();
}

ServerMonitor::ServerMonitor(std::map<int, WebServer *> servers) : _servers(servers), _sockets(_servers.size()){};

ServerMonitor::~ServerMonitor()
{
	for (std::map<int, WebServer *>::iterator itr = _servers.begin(); itr != _servers.end(); itr++)
		delete itr->second;
};

int ServerMonitor::retrieveClientHandlerSocket(int triggered)
{
	for (std::map<int, WebServer *>::iterator itr = _servers.begin(); itr != _servers.end(); itr++)
	{
		if (itr->second->connectedClient(triggered))
			return (itr->second->getConnection().getPassiveSocket());
	}
	return (-1);
}

static void closeCgiFds(const http::CGIhandler &cgi)
{
	dup2(cgi.getCinSave(), STDIN_FILENO);
	dup2(cgi.getCoutSave(), STDOUT_FILENO);
	close(cgi.getCinSave());
	close(cgi.getCoutSave());
	close(cgi.getInFileFd());
	close(cgi.getOutFileFd());
	fclose(const_cast<FILE *>(cgi.getInFile()));
	fclose(const_cast<FILE *>(cgi.getOutFile()));
}

void ServerMonitor::monitorCGI()
{
	std::clock_t curr_time;
	int clientSock;
	int serverSock;
	int status;

	for (std::vector<http::CGIhandler>::iterator it = _cgiScripts.begin(); it != _cgiScripts.end(); ++it)
	{
		curr_time = std::clock();
		if ((size_t)(curr_time - it->getStartTime()) >= ((size_t)(CLOCKS_PER_SEC)*TIME_OUT_SEC))
		{
			kill(it->getChildPid(), SIGKILL);
			clientSock = it->getClientSocket();
			serverSock = it->getServerSocket();
			closeCgiFds(*it);
			_cgiScripts.erase(it);
			_servers.at(serverSock)->responses[clientSock] = Response(408);
			return;
		}
		else
		{
			status = 0;
			int child = waitpid(it->getChildPid(), &status, WNOHANG);
			if (status == -1 || status == 1)
				throw(http::CGIhandler::CGIexception("CGI failed to run!"));
			if (child == it->getChildPid() || child == -1)
			{
				_servers.at((it->getServerSocket()))->closeCGI(*it, status);
				_cgiScripts.erase(it);
				return;
			}
		}
	}
}

void ServerMonitor::startListening()
{
	for (std::map<int, WebServer *>::iterator itr = _servers.begin(); itr != _servers.end(); itr++)
	{
		_sockets.addFd(itr->second->getConnection().getPassiveSocket(), POLLIN | POLLOUT);
		itr->second->startListening();
	}
}

bool ServerMonitor::incomingConnectiontoServer(int triggered)
{
	return (_servers.find(triggered) != _servers.end());
}

void ServerMonitor::acceptIncomingConnection(int triggered)
{
	int client = -1;
	try
	{
		client = _servers.find(triggered)->second->acceptConnection();
	}
	catch (ServerSocket::SocketIOError &e)
	{
		std::cerr << "Failed to accept incoming connection from a client: " << e.what() << std::endl;
		return;
	}
	if (_sockets.getNfds() < MAX_SOCKETS)
		_sockets.addFd(client, POLLIN | POLLOUT | POLLHUP | POLLERR);
	else
		closeClientConnection(triggered, client);
}

void ServerMonitor::closeClientConnection(int server, int client)
{
	_servers.at(server)->closeClientConnection(client);
	_sockets.removeFd(client);
}

void ServerMonitor::serveClientRequest(int server, int client)
{
	if (server == -1)
		return;
	int status = _servers.at(server)->recieveData(client);
	if (status == -1)
	{
		closeClientConnection(server, client);
		return;
	}
	if (_servers.at(server)->requestReady(client) == false)
		return;
	try
	{
		_servers.at(server)->buildResponse(client);
	}
	catch (http::CGIhandler &cgi)
	{
		cgi.setServerSocket(server);
		cgi.setClientSocket(client);
		_cgiScripts.push_back(cgi);
		_servers.at(server)->responses[client].setResponseStatus(false);
	}
	_servers.at(server)->responses[client].setPacketStatus(NOT_STARTED);
	std::map<int, Request>::iterator itr = _servers.at(server)->requests.find(client);
	_servers.at(server)->requests.erase(itr);
}

void ServerMonitor::serveClientResponse(int server, int client, int &requests)
{
	size_t bytesSent = 0;
	size_t bytesToSend = 0;

	if (server == -1 || _servers.at(server)->responseReady(client) == false)
		return;
	bytesToSend = http::getFileSize(_servers.at(server)->responses[client].getRawData());
	try
	{
		bytesSent = _servers.at(server)->sendResponse(client, (_servers.at(server)->responses[client]));
	}
	catch (ServerSocket::SocketIOError &e)
	{
		std::cerr << "Failed to serve client with a response: " << e.what() << std::endl;
		closeClientConnection(server, client);
		return;
	}
	if (bytesSent < 0)
	{
		closeClientConnection(server, client);
		return ;
	}
	if (bytesSent < bytesToSend)
		return;
	closeClientConnection(server, client);
	requests++;
	std::map<int, Response>::iterator response = _servers.at(server)->responses.find(client);
	_servers.at(server)->responses.erase(response);
}

void ServerMonitor::startServers()
{
	startListening();
	int rc = 0;
	int triggered = 0;
	int server = 0;
	int requests = 0;
	int i = 0;
	while (2)
	{
		monitorCGI();
		rc = _sockets.callPoll();
		if (rc < 0)
			throw ServerSocket::SocketIOError();
		for (i = 0; i < _sockets.getNfds(); i++)
		{
			triggered = _sockets[i].fd;
			if (_sockets[i].revents == 0)
				continue;
			server = retrieveClientHandlerSocket(triggered);
			if ((_sockets[i].revents & POLLHUP) || (_sockets[i].revents & POLLERR))
				closeClientConnection(server, triggered);
			else if (_sockets[i].revents & POLLIN && incomingConnectiontoServer(triggered))
				acceptIncomingConnection(triggered);
			else if (_sockets[i].revents & POLLIN)
				serveClientRequest(server, triggered);
			else if ((_sockets[i].revents & POLLOUT))
				serveClientResponse(server, triggered, requests);
		}
	}
}