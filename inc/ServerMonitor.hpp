/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMonitor.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:13:10 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/18 17:54:06 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SERVER_MONITOR_HPP
# define FT_SERVER_MONITOR_HPP

#include "PollManager.hpp"
#include "WebServer.hpp"
#include <vector>
#include <map>

class ServerMonitor {

	PollManager					sockets;
	std::map<int, WebServer*>	servers;

	public:
	void startServers() ;
	int retrieveClientHandlerSocket(int);

	ServerMonitor(std::map<int, WebServer *> servers) ;
	~ServerMonitor();
};

#endif
