/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/02 20:14:35 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(std::string ip, int port) : connection(ip, port)
{
}

WebServer::~WebServer()
{
}

void WebServer::startConnection()
{
	connection.startConnection();
}

void WebServer::startListening()
{
	connection.startListening();
}

void WebServer::acceptConnection()
{
	connection.acceptConnection();
}

void WebServer::closeConnection()
{
	connection.closeConnection();
	close(connection.getPassiveSocket());
}

void WebServer::sendData(std::string message)
{
	connection.sendData(message);
}

std::string WebServer::recieveData()
{
	return (connection.recieveData());
}


ServerSocket &WebServer::getConnection()
{
	return (connection);
}
