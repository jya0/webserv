/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/04 01:45:13 by rriyas           ###   ########.fr       */
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
					std::cout << "Triggered = " << triggered<<std::endl;
					int status = _servers.at(server)->recieveData(triggered);
					if (triggered == -1)
						continue;
					if(status == 0)
					{
						//nothin to read - closed
						std::cout<<"done reading\n";
					}
					else {
						if (_servers.at(server)->requestReady(triggered))
						{
							_servers.at(server)->buildResponse(triggered);
							std::map<int, Request*>::iterator itr = _servers.at(server)->requests.find(triggered);
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
	}
}