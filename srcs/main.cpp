/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/30 14:28:47 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ServerConfig/HTTPServerParser.hpp"
#include "../inc/WebServer.hpp"
#include "../inc/PollManager.hpp"
#include "../inc/ServerMonitor.hpp"

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;
	WebServer s1 = WebServer("127.0.0.1", atoi(argv[1]));
	WebServer s2 = WebServer("127.0.0.1", atoi(argv[1]) + 1);
	std::map<int, WebServer*> servers;
	servers.insert(std::make_pair(s1.getConnection().getPassiveSocket(), &s1));
	servers.insert(std::make_pair(s2.getConnection().getPassiveSocket(), &s2));

	ServerMonitor monitor(servers);
	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	try
	{
		monitor.startServers();
		// servers = HTTPServerParser::parseConfigFile(argv[1]);
		// s1.startServer();

	}
	// catch (HTTPServerParser::ParseError &e)
	// {
	// 	// handle parse error
	// }
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