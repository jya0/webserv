/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/02 20:45:45 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServer.hpp"
#include "../inc/HTTPServerParser.hpp"

void StartServers(std::vector<WebServer> &servers)
{

	int kq = kqueue();
	if (kq == -1){
		//error
	}

	struct kevent events[4] = {0};

	EV_SET(events, servers[0].getConnection().getPassiveSocket(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, NULL);
	kevent(kq, events, 2, NULL, 0, NULL);
	servers[0].startListening();
	while (true)
	{
		int n = kevent(kq, NULL, 0, events, 1, NULL);
		for (int i = 0; i < n; i++)
		{
			std::cerr<<"*****************************************\n";
			if (events[i].flags & EV_EOF)
			{
				std::cout << "Client disconnected!\n";
				EV_SET(events, servers[0].getConnection().getPeerSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				EV_SET(events, servers[0].getConnection().getPassiveSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(kq, events, 2, NULL, 0, NULL);
			}
			else if (events[i].ident == servers[0].getConnection().getPassiveSocket())
			{
				servers[0].acceptConnection();
				EV_SET(events, servers[0].getConnection().getPeerSocket(), EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, NULL);
				kevent(kq, events, 1, NULL, 0, NULL);
			}
			else if (events[i].filter == EVFILT_READ)
			{
				servers[0].recieveData();
				servers[0].sendData("HOLAA");
				servers[0].getConnection().closeConnection();
			}
			else if (events[i].filter == EVFILT_WRITE)
			{
				std::cout<<"***\n\nhey******\n\n";
			}

		}
	}
	EV_SET(events, servers[0].getConnection().getPeerSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(kq, events, 1, NULL, 0, NULL);
	close(kq);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	WebServer s1 = WebServer("127.0.0.1", 8080);
	std::vector<WebServer> servers;
	servers.push_back(s1);
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