/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/08 14:53:18 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SERVER_MONITOR_HPP
#define FT_SERVER_MONITOR_HPP

#include <vector>
#include <map>
#include <signal.h>
#include <sys/wait.h>
#include "PollManager.hpp"
#include "WebServer.hpp"

#define TIME_OUT_SEC	5

class ServerMonitor
{
private:
	std::map<int, WebServer *>	_servers;
	PollManager					_sockets;
	std::map<int, CGIhandler>	_cgiScripts;

public:

	ServerMonitor(std::map<int, WebServer *> servers);
	ServerMonitor(const std::vector<ServerConfig> &configsREF);
	~ServerMonitor();

	void	startServers();
	int		retrieveClientHandlerSocket(int);
	void	monitorCGI(int server);
};

#endif
