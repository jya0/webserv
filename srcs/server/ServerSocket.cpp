/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 18:30:35 by jyao              #+#    #+#             */
/*   Updated: 2023/12/14 01:59:34 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include "ServerSocket.hpp"
#include "Header.hpp"

static void log(std::string mesg)
{
	std::cout << mesg << std::endl;
}

ServerSocket::ServerSocket()
{
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	passive_socket = -1;
	port = 8000;
}

ServerSocket::ServerSocket(std::string ip_addr, int port) : ip_address(ip_addr), port(port),
															passive_socket(-1), socket_address(),
															socket_address_len(sizeof(socket_address))
{
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	startConnection();
}

ServerSocket::ServerSocket(const ServerSocket &rhs)
{
	*this = rhs;
}

ServerSocket &ServerSocket::operator=(const ServerSocket &rhs)
{
	if (this == &rhs)
		return (*this);
	this->port = rhs.port;
	this->ip_address = rhs.ip_address;
	this->passive_socket = rhs.passive_socket;
	this->socket_address = rhs.socket_address;
	this->socket_address.sin_family = AF_INET;
	this->socket_address.sin_port = htons(port);
	this->socket_address_len = sizeof(socket_address);
	this->socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	return (*this);
}

ServerSocket::~ServerSocket()
{
	closeConnection();
}

std::string ServerSocket::recieveData(int &peer_socket)
{
	if (peer_socket == passive_socket)
		return ("");
	long bytesRecieved;
	char *buffer = new char[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE + 1);
	bytesRecieved = recv(peer_socket, buffer, BUFFER_SIZE, 0);
	if (bytesRecieved == 0)
	{
		delete[] buffer;
		return (std::string(""));
	}
	if (bytesRecieved < 0)
	{
		log("read() sys call failed: Failed to read bytes from client socket\n");
		delete[] buffer;
		throw SocketIOError();
	}
	std::cout << "------ Reading Request from client ------\n\n";
	std::string ret(buffer);
	delete[] buffer;
	return (ret);
}

void ServerSocket::sendData(int &peer_socket, std::string message)
{
	ssize_t bytesSent;

	const char *s = message.c_str();
	std::cerr << message;
	bytesSent = send(peer_socket, s, message.size(), 0);
	if (bytesSent < 0)
	{
		log("send() sys call failed: Failed to send bytes to client socket\n");
		throw SocketIOError();
	}
	if (size_t(bytesSent) == message.size())
		log("------ Server Response sent to client ------\n\n");
}

void ServerSocket::startConnection()
{
	passive_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (passive_socket < 0)
	{
		log("socket() sys call failed: Socket creation failed\n");
		throw SocketIOError();
	}
	if (bind(passive_socket, (sockaddr *)&socket_address, socket_address_len) < 0)
	{
		log("bind() sys call failed: Cannot bind socket to address\n");
		closeConnection();
		throw SocketIOError();
	}
	fcntl(passive_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
}

void ServerSocket::startListening()
{
	if (listen(passive_socket, 50) < 0)
	{
		log("listen() sys call failed: Cannot listen from server socket\n");
		close(passive_socket);
		passive_socket = -1;
		throw SocketIOError();
	}
	std::cout << "\n*** Listening on ADDRESS: "
			  << inet_ntoa(socket_address.sin_addr)
			  << " PORT: " << ntohs(socket_address.sin_port)
			  << " ***\n\n";
}

int ServerSocket::acceptConnection()
{
	int peer_socket = accept(passive_socket, (sockaddr *)&socket_address,
							 &socket_address_len);
	if (peer_socket < 0)
	{
		close(peer_socket);
		std::cout << "accept() sys call failed: Server failed to accept incoming connection from ADDRESS: "
				  << inet_ntoa(socket_address.sin_addr) << "; PORT: "
				  << ntohs(socket_address.sin_port);
		throw SocketIOError();
	}
	fcntl(peer_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	std::cout << "Connection accepted!\n";
	return (peer_socket);
}

void ServerSocket::closeConnection()
{
	if (passive_socket != -1)
	{
		close(passive_socket);
		passive_socket = -1;
	}
}

const int &ServerSocket::getPassiveSocket()
{
	return (passive_socket);
}

const char *ServerSocket::SocketIOError::what() const throw()
{
	return ("Socket IO error!\n");
}