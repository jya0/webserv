/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/10/29 06:32:50 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ServerConfig/HTTPServerParser.hpp"
#include "../inc/WebServer.hpp"
#include "../inc/PollManager.hpp"


int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	WebServer s1 = WebServer("127.0.0.1", 8080);

	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	try
	{
		// servers = HTTPServerParser::parseConfigFile(argv[1]);
		s1.startServer();
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