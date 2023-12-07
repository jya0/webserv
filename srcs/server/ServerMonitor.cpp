/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/07 18:01:50 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMonitor.hpp"

ServerMonitor::ServerMonitor(const std::vector<ServerConfig> &configsREF) : _sockets(configsREF.size())
{
	WebServer					*server;
	size_t i = 0;

	for (std::vector<ServerConfig>::const_iterator itr = configsREF.begin(); itr != configsREF.end(); itr++, i++)
	{
		server = new WebServer(*itr);
		_servers.insert(std::make_pair(server->getConnection().getPassiveSocket(), server));
	}
	_sockets = PollManager(_servers.size());
}

ServerMonitor::ServerMonitor(std::map<int, WebServer *> servers): _servers(servers), _sockets(_servers.size()) {};

ServerMonitor::~ServerMonitor() {};

int ServerMonitor::retrieveClientHandlerSocket(int triggered) {
	for (std::map<int, WebServer *>::iterator itr = _servers.begin(); itr != _servers.end(); itr++)
	{
		if (itr->second->connectedClient(triggered))
			return (itr->second->getConnection().getPassiveSocket());
	}
	return (-1);
}

void ServerMonitor::monitorCGI(int server) {
	std::clock_t curr_time;
	std::map<int, CGIhandler>::iterator itr;

	int status = 0;
	for (itr = _cgiScripts.begin(); itr != _cgiScripts.end();)
	{
		curr_time = std::clock();
		if ((curr_time - itr->second.getStartTime()) >= (CLOCKS_PER_SEC * 10))
		{
			kill(itr->second.getChildPid(), SIGKILL);
			_servers.at((itr->first))->closeCGI(itr->second);
			int sock = itr->second.getClientSocket();
			_cgiScripts.erase(itr++);
			*_servers.at(server)->responses[sock] = Response(408);
		}
		else
		{
			status = 0;
			waitpid(itr->second.getChildPid(), &status, WNOHANG);
			if (status == -1 || status == 1)
				throw(http::CGIhandler::CGIexception("CGI failed to run!"));
			if (WIFEXITED(status) == 0)
			{
				_servers.at((itr->first))->closeCGI(itr->second);
				_cgiScripts.erase(itr++);
			}
		}
		if (itr == _cgiScripts.end())
			break;
		itr++;
	}
}

void ServerMonitor::startServers()
{

	int i = 0;
	for (std::map<int, WebServer *>::iterator itr = _servers.begin(); itr != _servers.end(); itr++)
	{
		_sockets.addFd(itr->second->getConnection().getPassiveSocket(), POLLIN | POLLOUT);
		itr->second->startListening();
	}
	int rc;
	int triggered;
	int client;
	int server;

	i = 0;
	while (2)
	{
		rc = _sockets.callPoll();
		if (rc < 0)
		{
			perror("poll() failed");
			break;
		}

		for (i = 0; i < _sockets.getNfds(); i++)
		{
			triggered = _sockets[i].fd;
			server = retrieveClientHandlerSocket(triggered);
			if (_sockets[i].revents == 0)
				continue;
			else if ( server != -1 && ((_sockets[i].revents & POLLHUP) || (_sockets[i].revents & POLLERR)))
			{
				_servers.at(server)->closeClientConnection(triggered);
				_sockets.removeFd(triggered);
			}
			else if (_sockets[i].revents & POLLIN)
			{
				if (_servers.find(triggered) != _servers.end())
				{
					client = _servers.find(triggered)->second->acceptConnection();
					if (client != -1)
						_sockets.addFd(client, POLLIN | POLLOUT | POLLHUP | POLLERR);
				}
				else
				{
					_servers.at(server)->recieveData(triggered);
					if (triggered == -1)
						continue;
					else {
						if (_servers.at(server)->requestReady(triggered))
						{
							try
							{
								_servers.at(server)->buildResponse(triggered);
							}
							catch (http::CGIhandler &cgi)
							{
								cgi.setServerSocket(server);
								cgi.setClientSocket(triggered);
								_cgiScripts.insert(std::make_pair<int, CGIhandler>(server, cgi));
							}
							std::map<int, Request *>::iterator itr = _servers.at(server)->requests.find(triggered);
							_servers.at(server)->requests.erase(itr);
						}
					}
				}
			}
			else if (server != -1 && (_sockets[i].revents & POLLOUT) && _servers.at(server)->responseReady(triggered))
			{
				_servers.at(server)->sendResponse(triggered, *(_servers.at(server)->responses[triggered]));
				std::map<int, Response *>::iterator itr = _servers.at(server)->responses.find(triggered);
				_servers.at(server)->responses.erase(itr);
			}
		}
		monitorCGI(server);
	}
}