/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/03 13:42:35 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig/HTTPServerParser.hpp"


void StartServers(std::map<int, WebServer*> &servers)
{
	int kq = kqueue();
	if (kq == -1){
		//error
	}
	struct kevent events[40] = {0};
	for (std::map<int, WebServer*>::iterator itr = servers.begin(); itr != servers.end(); itr++)
		EV_SET(events, itr->second->getConnection().getPassiveSocket(), EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
	kevent(kq, events, servers.size(), NULL, 0, NULL);
	for (std::map<int, WebServer*>::iterator itr = servers.begin(); itr != servers.end(); itr++)
		itr->second->startListening();
	while (true)
	{
		int n = kevent(kq, NULL, 0, events, 1, NULL);
		for (int i = 0; i < n; i++)
		{
			std::cerr<<"*****************************************\n";
			int triggered = events[i].ident;
			if (servers.find(triggered) != servers.end() && events[i].flags & EV_EOF)
			{
				std::cout << "Client disconnected!\n";
				EV_SET(events, servers[triggered]->getConnection().getPeerSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				EV_SET(events, servers[triggered]->getConnection().getPassiveSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(kq, events, 2, NULL, 0, NULL);
			}
			if (servers.find(triggered) != servers.end() && servers.find(triggered)->second->getConnection().getPeerSocket() == -1)
			{
				servers[triggered]->acceptConnection();
				EV_SET(events, servers[triggered]->getConnection().getPeerSocket(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, NULL);
				// EV_SET(events, servers[triggered]->getConnection().getPeerSocket(), EVFILT_WRITE, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, NULL);
				kevent(kq, events, 1, NULL, 0, NULL);
				std::cerr<<"moving on...";
			}
			if (events[i].filter == EVFILT_READ && servers.find(triggered) != servers.end() && servers.find(triggered)->second->getConnection().getPeerSocket() != -1)
			{
				std::cerr<<events[triggered].ident<<std::endl;
				servers[triggered]->recieveData();
				Response *resp = new Response(ServerSocket::generateDefaultResponse());
				servers[triggered]->sendResponse(*resp);
				// servers[triggered]->getConnection().closeConnection();
			}
		}
	}
	for (std::map<int, WebServer*>::iterator itr = servers.begin(); itr != servers.end(); itr++)
		EV_SET(events, itr->second->getConnection().getPeerSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(kq, events, 1, NULL, 0, NULL);
	close(kq);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	WebServer s1 = WebServer("127.0.0.1", 8080);
	// WebServer s2 = WebServer("127.0.0.1", 4040);

	std::map<int, WebServer*> servers;
	servers.insert(std::make_pair(s1.getConnection().getPassiveSocket(), &s1));
	// servers.insert(std::make_pair(s2.getConnection().getPassiveSocket(), &s2));

	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	try
	{
		// servers = HTTPServerParser::parseConfigFile(argv[1]);
		StartServers(servers);
	}
	catch (HTTPServerParser::ParseError &e)
	{
		// handle parse error
	}
	catch(ServerSocket::SocketIOError)
	{
		// handle socket IO error
	}
	catch (...)
	{

	}

	// server.startListening();
	return (0);
}