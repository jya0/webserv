/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/04 05:47:47 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"WebServer.hpp"
#include	"PollManager.hpp"
#include	"ServerMonitor.hpp"
#include	"HTTPServerParser.hpp"
#include	"MimeTypes.hpp"

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
	catch(ServerSocket::SocketIOError)
	{
		// handle socket IO error
	}
	catch (...)
	{

	}
	return (0);
}