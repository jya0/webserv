/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/13 19:25:58 by rriyas           ###   ########.fr       */
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

#define TIME_OUT_SEC	10

class ServerMonitor
{
private:
	std::map<int, WebServer* >		_servers;
	PollManager						_sockets;
	std::vector<http::CGIhandler>	_cgiScripts;

public:

	ServerMonitor(std::map<int, WebServer *> servers);
	ServerMonitor(const std::vector<ServerConfig> &configsREF);
	~ServerMonitor();

	void	startServers();
	int		retrieveClientHandlerSocket(int);
	void	monitorCGI();
	void	startListening();
	bool	incomingConnectiontoServer(int triggered);
	void	acceptIncomingConnection(int triggered);
	void	closeClientConenction(int server, int client);
	void	serveClientRequest(int server, int client);
	void	serveClientResponse(int server, int client, int &requests);
};

#endif
