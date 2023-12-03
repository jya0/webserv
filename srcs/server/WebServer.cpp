/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/03 15:03:23 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"WebServer.hpp"
#include	"PollManager.hpp"

WebServer::WebServer() {
}
// WebServer::WebServer(const DirectiveBlock &serverBlockREF)
// {

// }

WebServer::WebServer(std::string ip, int port) : connection(ip, port) {
}

// WebServer::WebServer(DirectiveBlock const	&serverBlockREF): _serverConfig(&serverBlockREF)
// {
// }

WebServer::~WebServer() {
}

void WebServer::startConnection() {
	connection.startConnection();
}

void WebServer::startListening() {
	connection.startListening();
}

int WebServer::acceptConnection() {
	int client;

	client = connection.acceptConnection();
	clients.push_back(client);
	return (client);
}

void WebServer::closeServerConnection() {
	connection.closeConnection();
}

void WebServer::closeClientConnection(int client) {
	if (clients.size() == 0)
		return ;
	if (std::find(clients.begin(), clients.end(), client) != clients.end())
	{
		close(client);
		clients.erase(std::find(clients.begin(), clients.end(), client));
	}
}


void WebServer::sendData(int client, std::string message) {
	connection.sendData(client, message);
}

int WebServer::recieveData(int &client)
{
	std::string ret = connection.recieveData(client);
	std::map<int, Request *>::iterator itr = requests.find(client);
	if (itr == requests.end())
		requests[client] = new Request();
	requests[client]->appendRawData(ret);
	requests[client]->setRequestStatus(false);
	if (requests[client]->getRawData().find("\r\n\r\n") != std::string::npos)
	{
		requests[client]->setRequestStatus(true);
		std::cout << "------ Finished Reading Request from client completely------\n\n";
		std::cout<<requests[client]->getRawData()<<"\n";

	}
	return (ret.size());
}


ServerSocket &WebServer::getConnection() {
	return (connection);
}

Request WebServer::receiveRequest(int client, std::string rawRequest) {
    Request request(rawRequest);
    if (!request.validate()) {
        sendResponse(client, Response(400));
    }
    return (request);
}

void WebServer::sendResponse(int client, const Response &response) {
	std::string rawResponse = response.getRawMessage();
    sendData(client, rawResponse);
}

void WebServer::prepareResponse(int client) {
	responses[client] = new Response(ServerSocket::generateDefaultResponse());
	responses[client]->setResponseStatus(true);
}

bool WebServer::connectedClient(int client) const{
	return (std::find(clients.begin(), clients.end(), client) != clients.end());
}

bool WebServer::responseReady(int client){
	if (responses.count(client) == 0)
		return (false);
	return (responses[client]->responseReady());
}

bool WebServer::requestReady(int client){
	if (requests.count(client) == 0)
		return (false);
	return (requests[client]->requestReady());
}

void WebServer::buildResponse(int client) {
	responses[client] = new Response(ServerSocket::generateDefaultResponse());
	//convert request to response
	responses[client]->setResponseStatus(true);
}