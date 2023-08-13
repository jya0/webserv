/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:14:16 by rriyas            #+#    #+#             */
/*   Updated: 2023/08/13 19:00:55 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/webserv.hpp"

static void log(std::string mesg)
{
	std::cout << mesg << std::endl;
}

TCPServer::TCPServer()
{
	m_socketAddress.sin_family = AF_INET;
	m_socketAddress.sin_port = htons(m_port);
	m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
	startServer();
}

TCPServer::TCPServer(std::string ip_addr, int port) : m_ip_address(ip_addr), m_port(port),
													  m_socket(), m_new_socket(), m_incomingMessage(), m_socketAddress(),
													  m_socketAddress_len(sizeof(m_socketAddress)), m_serverMessage(buildResponse())
{
	m_socketAddress.sin_family = AF_INET;
	m_socketAddress.sin_port = htons(m_port);
	m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
	startServer();
}

void TCPServer::startListening()
{
	if (listen(m_socket, 20) < 0)
	{
		log("Socket listen failed\n");
	}
	std::ostringstream ss;
	ss << "\n*** Listening on ADDRESS: "
	   << inet_ntoa(m_socketAddress.sin_addr)
	   << " PORT: " << ntohs(m_socketAddress.sin_port)
	   << " ***\n\n";
	log(ss.str());

	int bytesRecieved;

	while (2)
	{
		log("_________WAITING FOR A NEW CONNECTION__________");
		acceptConnection(m_new_socket);

		char buffer[BUFFER_SIZE] = {0};
		bytesRecieved = read(m_new_socket, buffer, BUFFER_SIZE);
		if (bytesRecieved < 0)
		{
			log("Failed to read bytes from client socket connection\n");
			exit(0);
		}

		std::ostringstream ss;
		ss << "------ Received Request from client ------\n\n";
		log(ss.str());

		sendResponse();

		close(m_new_socket);
	}
}

void TCPServer::acceptConnection(int &new_socket)
{
	new_socket = accept(m_socket, (sockaddr *)&m_socketAddress,
						&m_socketAddress_len);
	if (new_socket < 0)
	{
		std::ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: "
		   << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: "
		   << ntohs(m_socketAddress.sin_port);
		log(ss.str());
	}
}

void TCPServer::sendResponse()
{
	long bytesSent;

	bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

	if (bytesSent == m_serverMessage.size())
		log("------ Server Response sent to client ------\n\n");
	else
		log("Error sending response to client");
}

std::string TCPServer::buildResponse()
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
	   << htmlFile;
	return ss.str();
}

TCPServer::~TCPServer()
{
	closeServer();
}

int TCPServer::startServer()
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		log("Socket creation failed\n");
		return (1);
	}
	if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
	{
		log("Cannot connect socket to address");
		return (1);
	}
	return m_socket;
}

void TCPServer::closeServer()
{
	close(m_socket);
	close(m_new_socket);
	exit(0);
}