/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/12 23:06:17 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include "WebServer.hpp"
#include "PollManager.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(const ServerConfig &configREF) : connection(configREF.getListen().first, configREF.getListen().second), _config(configREF)
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

int WebServer::acceptConnection()
{
	int	client;

	client = connection.acceptConnection();
	if (client >= 0)
		clients.push_back(client);
	return (client);
}

void WebServer::closeServerConnection()
{
	connection.closeConnection();
}

void WebServer::closeClientConnection(int client)
{
	if (clients.size() == 0)
		return;
	if (std::find(clients.begin(), clients.end(), client) != clients.end())
	{
		close(client);
		clients.erase(std::find(clients.begin(), clients.end(), client));
	}
}

void WebServer::sendData(int client, std::string message)
{
	connection.sendData(client, message);
}

int WebServer::recieveData(int &client)
{
	std::string ret = connection.recieveData(client);
	std::map<int, Request>::iterator itr = requests.find(client);
	if (itr == requests.end())
		requests[client] = Request();
	requests[client].appendRawData(ret);
	requests[client].setRequestStatus(false);
	size_t pos = 0;
	if ((pos = requests[client].getRawData().find("\r\n\r\n")) != std::string::npos)
	{
		std::cout << "------ Finished Reading Request from client completely------\n\n";
		std::cout << requests[client].getRawData() << "\n";
		std::string raw = requests[client].getRawData();
		std::map<int, Request >::iterator itr = requests.find(client);
		requests.erase(itr);
		requests[client] = Request(raw);
		requests[client].setRequestStatus(true);
	}
	return (ret.size());
}

ServerSocket &WebServer::getConnection()
{
	return (connection);
}

Request WebServer::receiveRequest(int client, std::string rawRequest)
{
	Request request(rawRequest);
	if (!request.validate())
		sendResponse(client, Response(400));
	return (request);
}

void WebServer::sendResponse(int client, const Response &response)
{
	std::string rawResponse = response.getRawMessage();
	sendData(client, rawResponse);
}
bool WebServer::connectedClient(int client) const
{
	return (std::find(clients.begin(), clients.end(), client) != clients.end());
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

void WebServer::closeCGI(CGIhandler &cgiREF)
{
	std::string cgiResult;
	char *readBuf;
	ssize_t readReturn;
	try
	{
		readBuf = new char[READ_BUF_SIZE + 1];
	}
	catch (std::exception &e)
	{
		std::cerr << e.what()
				  << std::endl;
		throw(CGIhandler::CGIexception("CGI failed to read result!"));
	}
	lseek(cgiREF.getOutFileFd(), 0, SEEK_SET);
	do
	{
		std::memset(readBuf, 0, READ_BUF_SIZE + 1);
		readReturn = read(cgiREF.getOutFileFd(), readBuf, READ_BUF_SIZE);
		cgiResult += readBuf;
	} while (readReturn > 0);
	dup2(cgiREF.getCinSave(), STDIN_FILENO);
	dup2(cgiREF.getCoutSave(), STDOUT_FILENO);
	close(cgiREF.getCinSave());
	close(cgiREF.getCoutSave());
	close(cgiREF.getInFileFd());
	close(cgiREF.getOutFileFd());
	fclose(cgiREF.getInFile());
	fclose(cgiREF.getOutFile());

	responses[cgiREF.getClientSocket()] = Response(200, cgiResult);
	delete []readBuf;
}

void WebServer::buildResponse(int client)
{
	responses[client] = Response();
	responses[client].setResponseStatus(false);
	responses[client].buildResponse(requests[client], _config);
	responses[client].setResponseStatus(true);
}