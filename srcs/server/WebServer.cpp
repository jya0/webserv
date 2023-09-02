/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 17:55:39 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/02 15:57:20 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/WebServer.hpp"

WebServer::WebServer() {
}

WebServer::WebServer(std::string ip, int port) {
    // @todo
}

WebServer::~WebServer() {
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
    // @todo: SEND HERE
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
    // @todo: The HTTP GET method requests a representation of the specified
    // resource. Requests using GET should only be used to request data
    // (they shouldn't include data).

    // Flow:
    // 1. Check if the URI is a redirection
    //  1.1 If it is, return a 301
    // 2. Check if the URI is a file
    //  2.1 If it is, return the file
    // 3. Check if the URI is a directory
    //  3.1 If it is, respond according to config
    // 4. Check if the URI is a CGI script
    //  4.1 If it is, handle CGI
    // 5. Return 404
    return (response);
}

Response WebServer::handlePost(const Request &request) {
    Response response(200);
    // @todo: The HTTP POST method sends data to the server. The type of the
    // body of the request is indicated by the Content-Type header.
    return (response);
}

Response WebServer::handlePut(const Request &request) {
    Response response(200);
    // @todo: The HTTP PUT request method creates a new resource or replaces a
    // representation of the target resource with the request payload.
    return (response);
}

Response WebServer::handleDelete(const Request &request) {
    Response response(200);
    // @todo: The HTTP DELETE request method deletes the specified resource.

    // Flow:
    // 1. WHAT TO DO IF URI WAS A REDIRECT
    // 2. Check if the URI is a file
    //  2.1 If it is, delete the file
    // 3. Check if the URI is a directory
    //  3.1 If it is, respond according to config
    // 4. Check if the URI is a CGI script
    //  4.1 If it is, handle CGI
    return (response);
}

Response WebServer::handleHead(const Request &request) {
    Response response(200);
    // @todo: The HTTP HEAD method requests the headers that would be returned
    // if the HEAD request's URL was instead requested with the HTTP GET method.
    // For example, if a URL might produce a large download, a HEAD request
    // could read its Content-Length header to check the filesize without
    // actually downloading the file.
    return (response);
}