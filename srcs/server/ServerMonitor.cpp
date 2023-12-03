/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/03 13:54:02 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMonitor.hpp"

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

void ServerMonitor::startServers() {

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
					_servers.at(server)->recieveData(triggered);
					// build request (data)
					// if request complete -> build response

					if (triggered != -1)
						_servers.at(server)->prepareResponse(triggered);
					else
						continue;
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