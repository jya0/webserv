/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/16 09:10:53 by jyao             ###   ########.fr       */
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
	if (_servers.size() == 0)
	{
		std::cerr<<"All servers failed to start. Please update the config file with new ips or ports!\n";
		throw ServerSocket::SocketIOError();
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
	close(fileno(const_cast<FILE *>(cgi.getInFile())));
	close(fileno(const_cast<FILE *>(cgi.getOutFile())));
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
	int	client;

	client = _servers.find(triggered)->second->acceptConnection();
	if (client < 0)
	{
		std::cerr << "accept() sys call failed: Server failed to accept incoming connection from ADDRESS: "<< std::endl;
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
	ssize_t bytesRead;
	
	if (server == -1 || _servers.at(server)->requestReady(client) == true)
		return;
	std::map<int, Request>::iterator itr = _servers.at(server)->requests.find(client);
	bytesRead = _servers.at(server)->recieveData(client);
	if (bytesRead <= 0)
	{
		closeClientConnection(server, client);
		if (bytesRead == -1)
			std::cerr << "recv() sys call failed: Failed to read bytes from client socket\n"<< std::endl;
		else if (bytesRead == 0)
			std::cerr<< "Nothing left to read. Closing socket now...\n"<<std::endl;
		if (itr != _servers.at(server)->requests.end())
			_servers.at(server)->requests.erase(itr);
		return ;
	}
	if (_servers.at(server)->requestReady(client) == false)
		return ;
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
	itr = _servers.at(server)->requests.find(client);
	_servers.at(server)->requests.erase(itr);
}

void ServerMonitor::serveClientResponse(int server, int client, int &requests)
{
	ssize_t bytesSent = 0;

	if (server == -1 ||  _servers.at(server)->responseReady(client) == false)
		return;
	std::map<int, Response>::iterator itr = _servers.at(server)->responses.find(client);
	bytesSent = _servers.at(server)->sendResponse(client, _servers.at(server)->responses[client]);
	if (bytesSent < 0)
	{
		std::cerr << "send() sys call failed: Failed to send bytes to client socket\n" << std::endl;
		closeClientConnection(server, client);
		if (itr != _servers.at(server)->responses.end())
			_servers.at(server)->responses.erase(itr);
		return;
	}
	FILE *file = _servers.at(server)->responses[client].getMessageBody();
	if (file != NULL && !feof(file))
		return ;
	closeClientConnection(server, client);
	requests++;
	itr = _servers.at(server)->responses.find(client);
	_servers.at(server)->responses.erase(itr);
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