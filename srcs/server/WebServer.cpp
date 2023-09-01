/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/01 17:56:50 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"

WebServer::WebServer() : TCPServer()
{
}

WebServer::WebServer(std::string ip, int port) : TCPServer(ip, port)
{
}

void WebServer::InitiateServer()
{
	StartListening();
}

WebServer::~WebServer()
{
}
