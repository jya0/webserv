/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:53:34 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/18 20:24:36 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ServerMonitor.hpp"

ServerMonitor::ServerMonitor(std::map<int, WebServer *> servers) : servers(servers), sockets(servers.size())
{
}
ServerMonitor::~ServerMonitor()
{
}
int ServerMonitor::retrieveClientHandlerSocket(int triggered)
{
	for (std::map<int, WebServer *>::iterator itr = servers.begin(); itr != servers.end(); itr++)
	{
		if (itr->second->connectedClient(triggered))
			return (itr->second->getConnection().getPassiveSocket());
	}
	return (-1);
}

void ServerMonitor::startServers()
{

	int i = 0;
	for (std::map<int, WebServer *>::iterator itr = servers.begin(); itr != servers.end(); itr++)
	{
		sockets.addFd(itr->second->getConnection().getPassiveSocket(), POLLIN);
		itr->second->startListening();
	}
	int rc;
	int triggered;
	int client;
	int server;
	i = 0;

	while (2)
	{
		rc = sockets.callPoll();
		if (rc < 0)
		{
			perror("poll() failed");
			break;
		}

		for (i = 0; i < sockets.getNfds(); i++)
		{
			triggered = sockets[i].fd;
			server = retrieveClientHandlerSocket(triggered);
			if (sockets[i].revents == 0)
				continue;
			else if ( server != -1 && ((sockets[i].revents & POLLHUP) || (sockets[i].revents & POLLERR)))
			{
				servers.at(server)->closeClientConnection(triggered);
				sockets.removeFd(triggered);
			}
			else if (sockets[i].revents & POLLIN)
			{
				if (servers.find(triggered) != servers.end() && servers.find(triggered)->second->getConnection().getPassiveSocket() == triggered)
				{
					client = servers.find(triggered)->second->acceptConnection();
					if (client != -1)
						sockets.addFd(client, POLLIN | POLLOUT | POLLHUP | POLLERR);
				}
				else
				{
					std::cout << "Triggered = " << triggered;
					servers.at(server)->recieveData(&triggered);
					if (triggered != -1)
						servers.at(server)->prepareResponse(triggered);
					else
						continue;
				}
			}
			else if ((sockets[i].revents & POLLOUT) && server != -1 && servers.at(server)->responseReady(triggered))
			{
				servers.at(server)->sendResponse(triggered, *(servers.at(server)->responses[triggered]));
				std::map<int, Response *>::iterator itr = servers.at(server)->responses.find(triggered);
				servers.at(server)->responses.erase(itr);
			}
		}
	}
}