/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/03 13:47:21 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"
#include <fstream>

WebServer::WebServer()
{
}

WebServer::WebServer(std::string ip, int port) : connection(ip, port)
{
}

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

Request WebServer::receiveRequest(std::string rawRequest) {
    Request request(rawRequest);
    if (!request.validate()) {
        sendResponse(Response(400));
    }
    return (request);
}

void WebServer::sendResponse(const Response &response) { 
    std::string rawResponse = response.getRawMessage();
    sendData(rawResponse);
}

Response WebServer::handleRequest(const Request &request) {
	for (std::vector<std::pair<std::string, std::string> >::iterator it = redirections.begin(); it != redirections.end(); ++it)
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

Response WebServer::handleGet(const Request &request) {
    Response response(200);
	std::string filePath = root + request.getUri();
    //@todo: The HTTP GET method requests a representation of the specified resource. Requests using GET should only be used to
	// request data (they shouldn't include data).

    // Flow:
    std::ifstream file(filePath.c_str());
	if (file.good()) {
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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

Response WebServer::handlePost(const Request &request) {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handlePut(const Request &request) {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handleDelete(const Request &request) {
    Response response(200);
    // @todo
    return (response);
}

Response WebServer::handleHead(const Request &request) {
    Response response(200);
    // @todo
    return (response);
}