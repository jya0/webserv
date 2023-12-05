/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/06 01:16:59 by jyao             ###   ########.fr       */
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

		ServerMonitor(std::map<int, WebServer *> servers) ;
		ServerMonitor(const std::vector<ServerConfig> &configsREF);
		~ServerMonitor();
};

#endif
