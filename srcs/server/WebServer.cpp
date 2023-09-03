/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/03 10:12:05 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"

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
    // @todo
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