/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/13 11:49:23 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"WebServer.hpp"
#include	"PollManager.hpp"
#include	"ServerMonitor.hpp"
#include	"ServerSocket.hpp"
#include	"ServerParser_namespace.hpp"


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	try
	{
		std::vector<ServerConfig> configs = ServerParser::parseConfigFile(argv[1]);
		ServerMonitor monitor(configs);
		monitor.startServers();
	}
	catch(std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	return (0);
}