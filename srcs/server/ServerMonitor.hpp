/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/04 03:24:03 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		FT_SERVER_MONITOR_HPP
# define	FT_SERVER_MONITOR_HPP

#include	<vector>
#include	<map>
#include	"PollManager.hpp"
#include	"WebServer.hpp"
#include	<signal.h>
#include <sys/wait.h>

class ServerMonitor {
	private:
		std::map<int, WebServer*>	_servers;
		PollManager					_sockets;
		std::map<int, CGIhandler> _cgiScripts;

	public:
		void startServers() ;
		int retrieveClientHandlerSocket(int);

		ServerMonitor(std::map<int, WebServer *> servers) ;
		ServerMonitor(const std::vector<ServerConfig> &configsREF);
		~ServerMonitor();
};

#endif
