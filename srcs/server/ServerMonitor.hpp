/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/07 18:00:36 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		FT_SERVER_MONITOR_HPP
# define	FT_SERVER_MONITOR_HPP

#include	<vector>
#include	<map>
#include	<signal.h>
#include	<sys/wait.h>
#include	"PollManager.hpp"
#include	"WebServer.hpp"

class ServerMonitor {
	private:
		std::map<int, WebServer*>	_servers;
		PollManager					_sockets;
		std::map<int, CGIhandler>	_cgiScripts;

	public:
		void startServers();
		int retrieveClientHandlerSocket(int);
		void monitorCGI(int server);
		ServerMonitor(std::map<int, WebServer *> servers) ;
		ServerMonitor(const std::vector<ServerConfig> &configsREF);
		~ServerMonitor();
};

#endif
