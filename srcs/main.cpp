/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/10 19:32:04 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"WebServer.hpp"
#include	"PollManager.hpp"
#include	"ServerMonitor.hpp"
#include	"ServerSocket.hpp"
#include	"ServerParser_namespace.hpp"

int main(int argc, char **argv, char **envp)
{
	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}


	for (int i  = 0; envp[i]; i++) {
		std::cout<<envp[i]<<std::endl;
	}
	std::vector<ServerConfig> configs = ServerParser::parseConfigFile(argv[1]);
	ServerMonitor monitor(configs);
	try
	{
		monitor.startServers();
	}
	catch(ServerSocket::SocketIOError &e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	return (0);
}