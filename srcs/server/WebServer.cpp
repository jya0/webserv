/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/14 21:33:07 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include "WebServer.hpp"
#include "PollManager.hpp"
#include "Header.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(const ServerConfig &configREF) : connection(configREF.getListen().first, configREF.getListen().second), _config(configREF)
{
}

WebServer::WebServer(std::string ip, int port) : connection(ip, port)
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
	_connection		= rhs._connection;
	_clients		= rhs._clients;
	_config			= rhs._config;
	responses		= rhs.responses;
	requests		= rhs.requests;
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
	int	client;

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

void WebServer::sendData(int client, std::string message)
{
	_connection.sendData(client, message);
}

ssize_t	WebServer::recieveData(int &client)
{
	char		*buffer;
	std::string	bufSTR;
	ssize_t		bytesRead;

	buffer = NULL;
	try {
		bytesRead = _connection.recieveData(client, buffer);
		bufSTR = std::string(buffer, bytesRead);
		delete [](buffer);
	}
	catch(std::exception &e) {
		std::cerr << "Failed to receive client request: " << e.what() << std::endl;
		return (-1);
	}
	if (bytesRead == 0)
	{
		closeClientConnection(client);
		return (-1);
	}

	std::map<int, Request>::iterator itr = requests.find(client);
	if (itr == requests.end())
		requests[client] = Request();
	requests[client].appendRawData(bufSTR);
	requests[client].setRequestStatus(false);
	size_t pos = 0;
	if ((pos = requests[client].findRawData(CR_LF CR_LF)) != std::string::npos)
	{
		std::cout << "------ Finished Reading Request from client completely------\n\n";
		requests.erase(requests.find(client));
		requests[client] = Request(requests[client].getRawData());
		requests[client].setRequestStatus(true);
	}
	delete [](buffer);
	return (status);
}

ServerSocket &WebServer::getConnection()
{
	return (_connection);
}

void WebServer::sendResponse(int client, const Response &response)
{
	std::string rawResponse = response.getRawMessage();
	sendData(client, rawResponse);
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
	std::string	cgiResult;
	char		 *readBuf;
	ssize_t		readReturn;
	int			responseStatus;

	readReturn = 0;
	readBuf = NULL;
	responseStatus = 200;
	if (WEXITSTATUS(statusREF) != 0)
		responseStatus = 500;
	else {
		try {
			readBuf = new char[READ_BUF_SIZE + 1];
			lseek(cgiREF.getOutFileFd(), 0, SEEK_SET);
			do
			{
				std::memset(readBuf, 0, READ_BUF_SIZE + 1);
				readReturn = read(cgiREF.getOutFileFd(), readBuf, READ_BUF_SIZE);
				cgiResult += readBuf;
			} while (readReturn > 0);
		}
		catch (std::exception &e) {
			responseStatus = 500;
		}
	}
	cgiREF.closeParentFds();
	if (responseStatus != 200)
		cgiResult = "OOOOOOOPS I'VE FAILED CGI!!!";
	responses[cgiREF.getClientSocket()] = Response(responseStatus, cgiResult);
	delete []readBuf;
}

void WebServer::buildResponse(int client)
{
	responses[client] = Response();
	responses[client].setResponseStatus(false);
	responses[client].buildResponse(requests[client], _config);
	responses[client].setResponseStatus(true);
}
