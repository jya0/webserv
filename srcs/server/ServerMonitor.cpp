/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/14 04:28:33 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMonitor.hpp"

ServerMonitor::ServerMonitor(const std::vector<ServerConfig> &configsREF) : _sockets(configsREF.size()), _cgiScripts()
{
	size_t i = 0;
	WebServer *server;
	int n = 0;
	for (std::vector<ServerConfig>::const_iterator itr = configsREF.begin(); itr != configsREF.end(); itr++, i++)
	{
		try {
			server = new WebServer(*itr);
			_servers.insert(std::make_pair(server->getConnection().getPassiveSocket(), server));
			n++;
		}
		catch(ServerSocket::SocketIOError &e) {
			std::cerr<<e.what()<<std::endl;
		}
	}
	_sockets = PollManager(n);
	http::CGIhandler::setPollManager(_sockets);
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

static void closeCgiFds(http::CGIhandler &cgi)
{
	dup2(cgi.getCinSave(), STDIN_FILENO);
	dup2(cgi.getCoutSave(), STDOUT_FILENO);
	close(cgi.getCinSave());
	close(cgi.getCoutSave());
	close(cgi.getInFileFd());
	close(cgi.getOutFileFd());
	fclose(cgi.getInFile());
	fclose(cgi.getOutFile());
}

void ServerMonitor::monitorCGI()
{
	std::clock_t curr_time;
	int clientSock = 0;
	int serverSock = 0;

	int status = 0;
	if (_cgiScripts.size() == 0)
		return ;
	for (size_t i = 0; i < _cgiScripts.size(); i++)
	{
		curr_time = std::clock();
		if ((size_t)(curr_time - _cgiScripts[i].getStartTime()) >= ((size_t)(CLOCKS_PER_SEC)*TIME_OUT_SEC))
		{
			kill(_cgiScripts[i].getChildPid(), SIGKILL);
			clientSock = _cgiScripts[i].getClientSocket();
			serverSock = _cgiScripts[i].getServerSocket();
			closeCgiFds(_cgiScripts[i]);
			_cgiScripts.erase(_cgiScripts.begin() + i);
			_servers.at(serverSock)->responses[clientSock] = Response(408);
			return;
		}
		else
		{
			status = 0;
			int child = waitpid(_cgiScripts[i].getChildPid(), &status, WNOHANG);
			if (status == -1 || status == 1)
				throw(http::CGIhandler::CGIexception("CGI failed to run!"));
			if (child == _cgiScripts[i].getChildPid() || child == -1)
			{
				_servers.at((_cgiScripts[i].getServerSocket()))->closeCGI(_cgiScripts[i], status);
				_cgiScripts.erase(_cgiScripts.begin() + i);
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
	if (client != -1)
		_sockets.addFd(client, POLLIN | POLLOUT | POLLHUP | POLLERR);
}

void ServerMonitor::closeClientConnection(int server, int client)
{
	_servers.at(server)->closeClientConnection(client);
	_sockets.removeFd(client);
}

void ServerMonitor::serveClientRequest(int server, int client)
{
	if (server == -1)
		return ;
	int status = _servers.at(server)->recieveData(client);
	if (status == -1)
	{
		closeClientConnection(server, client);
		return ;
	}
	if (_servers.at(server)->requestReady(client) == false)
		return;
	try {
		_servers.at(server)->buildResponse(client);
	}
	catch (http::CGIhandler &cgi)
	{
		cgi.setServerSocket(server);
		cgi.setClientSocket(client);
		_cgiScripts.push_back(cgi);
		_servers.at(server)->responses[client].setResponseStatus(false);
	}
	std::map<int, Request>::iterator itr = _servers.at(server)->requests.find(client);
	_servers.at(server)->requests.erase(itr);
}

void ServerMonitor::serveClientResponse(int server, int client, int &requests)
{
	if (server == -1 || _servers.at(server)->responseReady(client) == false)
		return;
	try
	{
		_servers.at(server)->sendResponse(client, (_servers.at(server)->responses[client]));
		closeClientConnection(server, client);
	}
	catch (ServerSocket::SocketIOError &e)
	{
		std::cerr << "Failed to serve client with a response: " << e.what() << std::endl;
		closeClientConnection(server, client);
		return;
	}
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
	while (requests != 10)
	{
		rc = _sockets.callPoll();
		if (rc < 0)
			throw ServerSocket::SocketIOError();
		for (i = 0; i < _sockets.getNfds(); i++)
		{
			triggered = _sockets[i].fd;
			if (_sockets[i].revents == 0 || triggered == -1)
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
		monitorCGI();
	}
}