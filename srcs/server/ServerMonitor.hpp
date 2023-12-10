/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/09 04:18:52 by rriyas           ###   ########.fr       */
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
	std::vector<http::CGIhandler>	_cgiScripts;

public:

	ServerMonitor(std::map<int, WebServer *> servers);
	ServerMonitor(const std::vector<ServerConfig> &configsREF);
	~ServerMonitor();

	void	startServers();
	int		retrieveClientHandlerSocket(int);
	void	monitorCGI();
};

#endif
