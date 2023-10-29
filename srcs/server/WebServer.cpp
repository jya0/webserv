/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/10/29 06:38:58 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"
#include "../../inc/PollManager.hpp"

#include <fstream>

WebServer::WebServer()
{
}
// WebServer::WebServer(const DirectiveBlock &serverBlockREF)
// {

// }

WebServer::WebServer(std::string ip, int port) : connection(ip, port)
{
}

// WebServer::WebServer(DirectiveBlock const	&serverBlockREF): _serverConfig(&serverBlockREF)
// {
// }

WebServer::~WebServer() {
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
	int client;

	client = connection.acceptConnection();
	clients.push_back(client);
	return (client);
}

void WebServer::closeServerConnection()
{
	connection.closeConnection();
}

void WebServer::closeClientConnection(int client)
{
	close(client);
}


void WebServer::sendData(int client, std::string message)
{
	connection.sendData(client, message);
}

std::string WebServer::recieveData(int client)
{
	return (connection.recieveData(client));
}


ServerSocket &WebServer::getConnection()
{
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

Response WebServer::handleRequest(const Request &request) const {
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = redirections.begin(); it != redirections.end(); ++it)
		if (it->first == request.getUri())
			return(Response("HTTP/1.1 301 Moved Permanently\r\nLocation: " + it->second + "\r\n\r\n"));
    switch (Request::methodEnum(request.getHttpMethod())) {
        case GET:
            return (handleGet(request));
        case POST:
            return (handlePost(request));
        case PUT:
            return (handlePut(request));
        case DELETE:
            return (handleDelete(request));
        case HEAD:
            return (handleHead(request));
        default:
            return (Response(501));
    }
}

Response WebServer::handleGet(const Request &request) const {
    Response response(200);
	std::string filePath = root + request.getUri();
    //@todo: The HTTP GET method requests a representation of the specified resource. Requests using GET should only be used to
	// request data (they shouldn't include data).

    // Flow:
    std::ifstream file(filePath.c_str());
	if (file.good()) {
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        Header length("Content-Length", std::to_string(str.length()));
        Header type("Content-Type", "text/html");
        Header server("Server", "webserv-kry");
        // Header date("Date", getDate());
        response.addHeader(length);
        response.addHeader(type);
        response.setMessageBody(str);
        file.close();
        return (response);
	}
    // 3. Check if the URI is a directory
    //  3.1 If it is, respond according to config
    // 4. Check if the URI is a CGI script
    //  4.1 If it is, handle CGI
    // 5. Return 404
    return (response);
}

Response WebServer::handlePost(const Request &request) const {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handlePut(const Request &request) const {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handleDelete(const Request &request) const {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handleHead(const Request &request) const {
    Response response(200);
    // @todo
    return (response);
}

bool WebServer::connectedClient(int client) const{
	write(1, "Hi\n", 3);
	return (std::find(clients.begin(), clients.end(), client) != clients.end());
}

bool WebServer::responseReady(int client){
	return (responses[client]->responseReady());
}

void WebServer::startServer() {
	PollManager sockets(1);
	int i = 0;
	sockets.addFd(this->getConnection().getPassiveSocket(), POLLIN);
	startListening();

	int rc;
	int triggered;
	int client;
	while (2) {
		rc = sockets.callPoll();
		if (rc < 0) {
			perror("poll() failed");
			break;
		}

		for (i = 0; i < sockets.getNfds(); i++) {
			triggered = sockets[i].fd;

			if (sockets[i].revents & POLLIN) {
				if (triggered == this->getConnection().getPassiveSocket()) {
					client = acceptConnection();
					if (client != -1)
						sockets.addFd(client, POLLIN | POLLOUT);
				}
				else {
					std::cout << "Triggered = " << triggered;
					recieveData(triggered);
					prepareResponse(triggered);
				}
			}
			else if ((sockets[i].revents & POLLOUT) && responseReady(triggered)) {
				sendResponse(triggered, *responses[triggered]);
				responses[triggered]->setResponseStatus(false);
				closeClientConnection(triggered);
				sockets.removeFd(triggered);
			}
		}
	}
}