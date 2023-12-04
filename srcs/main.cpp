/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/04 14:57:12 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"WebServer.hpp"
#include	"PollManager.hpp"
#include	"ServerMonitor.hpp"
#include	"ServerSocket.hpp"
#include	"HTTPServerParser.hpp"

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;
	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	std::vector<ServerConfig> configs = HTTPServerParser::parseConfigFile(argv[1]);
	ServerMonitor monitor(configs);
	try
	{
		monitor.startServers();
	}
	// catch (HTTPServerParser::ParseError &e)
	// {
	// 	// handle parse error
	// }
	catch(ServerSocket::SocketIOError &e)
	{
		// handle socket IO error
	}
	catch (...)
	{

	}
	return (0);
}