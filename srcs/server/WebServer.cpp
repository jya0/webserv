/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/15 05:51:26 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include "WebServer.hpp"
#include "PollManager.hpp"
#include "Header.hpp"
#include "Response.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(const ServerConfig &configREF) : _connection(configREF.getListen().first, configREF.getListen().second), _config(configREF)
{
}

WebServer::WebServer(std::string ip, int port) : _connection(ip, port)
{
}

WebServer::WebServer(const WebServer &rhs)
{
	*this = rhs;
}

WebServer &WebServer::operator=(const WebServer &rhs)
{
	if (this == &rhs)
		return (*this);
	_connection = rhs._connection;
	_clients = rhs._clients;
	_config = rhs._config;
	responses = rhs.responses;
	requests = rhs.requests;
	return (*this);
}

WebServer::~WebServer()
{
	_connection.closeConnection();
}

void WebServer::startConnection()
{
	_connection.startConnection();
}

void WebServer::startListening()
{
	_connection.startListening();
}

int WebServer::acceptConnection()
{
	int client;

	client = _connection.acceptConnection();
	if (client >= 0)
		_clients.push_back(client);
	return (client);
}

void WebServer::closeServerConnection()
{
	_connection.closeConnection();
}

void WebServer::closeClientConnection(int client)
{
	if (_clients.size() == 0 || client == -1)
		return;
	if (std::find(_clients.begin(), _clients.end(), client) != _clients.end())
	{
		close(client);
		_clients.erase(std::find(_clients.begin(), _clients.end(), client));
	}
}

ssize_t WebServer::sendData(int client, std::string &message)
{
	ssize_t bytesSent;

	bytesSent = _connection.sendData(client, message);
	return (bytesSent);
}

ssize_t WebServer::recieveData(int &client)
{
	char *buffer;
	std::string bufSTR;
	ssize_t bytesRead;

	buffer = NULL;
	try
	{
		buffer = new char[BUFFER_SIZE + 1];
		memset(buffer, 0, BUFFER_SIZE + 1);
		bytesRead = _connection.recieveData(client, buffer);
		bufSTR = std::string(buffer, bytesRead);
		delete[] (buffer);
	}
	catch (std::exception &e)
	{
		std::cerr << "Failed to receive client request: " << e.what() << std::endl;
		return (-1);
	}
	if (bytesRead == 0)
	{
		closeClientConnection(client);
		return (-1);
	}
	if (!requests.count(client))
	{
		requests[client] = Request(bufSTR);
		requests[client].setRequestStatus(false);
	}
	requests[client].appendRawData(bufSTR);
	if (requests[client].recievedEOF())
	{
		std::cout << "------ Finished Reading Request from client completely------\n\n";
		requests.erase(requests.find(client));
		requests[client].parseRequest();
		requests[client].setRequestStatus(true);
	}
	return (bytesRead);
}

ServerSocket &WebServer::getConnection()
{
	return (_connection);
}

static char*	getNextPacket(Response &response)
{
	ssize_t bytesRead = 0;

	FILE *file = response.getRawMessage().second;
	int startPos = response.getPacketStartPos();
	char *packet = new char[BUFFER_SIZE + 1];
	fseek(file, startPos, SEEK_SET);
	bytesRead = fread(packet, sizeof(char), BUFFER_SIZE, file);
	if (bytesRead > 0)
		response.movePacketStartPos(startPos + startPos);
	return (packet);
}

ssize_t WebServer::sendResponse(int client, Response &response)
{
	char		*packet;
	std::string	packetStr;
	ssize_t		bytesSent;

	bytesSent = 0;
	t_raw_message rawResponse = response.getRawMessage();
	if (response.getPacketStatus() == NOT_STARTED)
	{
		bytesSent = sendData(client, rawResponse.first);
		if (bytesSent < 0)
			throw (ServerSocket::SocketIOError());
		response.setPacketStatus(SENDING);
	}
	packet = getNextPacket(response);
	packetStr = std::string(packet);
	bytesSent += sendData(client, packetStr);
	delete []packet;
	return (bytesSent);
}

bool WebServer::connectedClient(int client) const
{
	return (std::find(_clients.begin(), _clients.end(), client) != _clients.end());
}

bool WebServer::responseReady(int client)
{
	if (responses.count(client) == 0)
		return (false);
	return (responses[client].responseReady());
}

bool WebServer::requestReady(int client)
{
	if (requests.count(client) == 0)
		return (false);
	return (requests[client].requestReady());
}

void WebServer::closeCGI(const CGIhandler &cgiREF, const int &statusREF)
{
	std::string cgiResult;
	char *readBuf;
	ssize_t readReturn;
	int responseStatus;

	readReturn = 0;
	readBuf = NULL;
	responseStatus = 200;
	if (WEXITSTATUS(statusREF) != 0)
		responseStatus = 500;
	else
	{
		try
		{
			readBuf = new char[READ_BUF_SIZE + 1];
			lseek(cgiREF.getOutFileFd(), 0, SEEK_SET);
			do
			{
				std::memset(readBuf, 0, READ_BUF_SIZE + 1);
				readReturn = read(cgiREF.getOutFileFd(), readBuf, READ_BUF_SIZE);
				cgiResult += readBuf;
			} while (readReturn > 0);
		}
		catch (std::exception &e)
		{
			responseStatus = 500;
		}
	}
	cgiREF.closeParentFds();
	if (responseStatus != 200)
		cgiResult = "OOOOOOOPS I'VE FAILED CGI!!!";
	responses[cgiREF.getClientSocket()] = Response(responseStatus, cgiResult);
	delete[] readBuf;
}

void WebServer::buildResponse(int client)
{
	responses[client] = Response();
	responses[client].setResponseStatus(false);
	responses[client].buildResponse(requests[client], _config);
	responses[client].setResponseStatus(true);
}
