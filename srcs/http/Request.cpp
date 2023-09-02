/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:38:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/09/02 13:44:15 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http_namespace.hpp"

using namespace http;

/**
 * @brief Construct a new Request object (default constructor)
 */
Request::Request(void) {
    return ;
}

/**
 * @brief Construct a new Request object (copy constructor)
 * 
 * @param RequestREF Request to copy
 */
Request::Request(Request const &RequestREF) {
    *this = RequestREF;
    return ;
}

/**
 * @brief Destroy the Request object
 */
Request::~Request(void) {
    return ;
}

/**
 * @brief The copy assignment operator of the Request class.
 * 
 * @param RequestREF Request to copy
 * @return Request& Reference to the copied Request
 */
Request	&Request::operator=(Request const &RequestREF) {
    if (this != &RequestREF) {
        this->_httpMethod = RequestREF._httpMethod;
        this->_uri = RequestREF._uri;
    }
    return (*this);
}

/**
 * @brief Construct a new Request object
 * 
 * @param httpRaw The raw HTTP request to be parsed
 */
Request::Request(std::string httpRaw): AMessage(httpRaw) {
    std::string method = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_httpMethod = this->methodEnum(method);
    this->_uri = this->_startLine.substr(this->_startLine.find(' ') + 1,
            this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
            this->_startLine.find(' ') - 1);
    this->_httpVersion = this->_startLine.substr(this->_startLine.find(' ',
                this->_startLine.find(' ') + 1) + 1);
}

/**
 * @brief Returns the HTTP method of the request.
 * 
 * @return std::string The HTTP method of the request.
 */
std::string	Request::getHttpMethod(void) {
    return (Request::methodName(this->_httpMethod));
}

/**
 * @brief Returns the HTTP version of the request.
 * 
 * @return std::string The HTTP version of the request.
 */
std::string	Request::getHttpVersion(void) {
    return (this->_httpVersion);
}

/**
 * @brief Returns the URI of the request.
 * 
 * @return std::string The URI of the request.
 */
std::string	Request::getUri(void) {
    return (this->_uri);
}

/**
 * @brief Swaps out the enum for the name of the HTTP method.
 * 
 * @param method The enum of the HTTP method.
 * @return std::string The name of the HTTP method.
 */
std::string Request::methodName(e_httpMethod method) {
    switch (method) {
        case GET:
            return ("GET");
        case POST:
            return ("POST");
        case HEAD:
            return ("HEAD");
        case PUT:
            return ("PUT");
        case DELETE:
            return ("DELETE");
        default:
            return ("");
    }
}

/**
 * @brief Swaps out the name for the enum of the HTTP method.
 * 
 * @param method The name of the HTTP method.
 * @return e_httpMethod The enum of the HTTP method.
 */
e_httpMethod Request::methodEnum(std::string method) {
    if (method == "GET")
        return (GET);
    else if (method == "POST")
        return (POST);
    else if (method == "HEAD")
        return (HEAD);
    else if (method == "PUT")
        return (PUT);
    else if (method == "DELETE")
        return (DELETE);
    else
        return (GET);
}

/**
 * @brief Validates the headers of the request.
 * 
 * @return true If the headers are valid.
 * @return false If the headers are invalid.
 */
bool	Request::validate(void) {
    return (true); /// @todo: implement
}