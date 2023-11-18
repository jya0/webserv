/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:38:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/11/18 16:15:11 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http_namespace.hpp"

using namespace http;

/**
 * @brief Construct a new Request object (default constructor)
 */
Request::Request(void) : AMessage()
{
	return ;
}

/**
 * @brief Construct a new Request object (copy constructor)
 *
 * @param RequestREF Request to copy
 */
Request::Request(Request const &RequestREF) : AMessage()
{
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
std::string	Request::getHttpMethod(void) const {
    return (Request::methodName(this->_httpMethod));
}

/**
 * @brief Returns the HTTP version of the request.
 *
 * @return std::string The HTTP version of the request.
 */
std::string	Request::getHttpVersion(void) const {
    return (this->_httpVersion);
}

/**
 * @brief Returns the URI of the request.
 *
 * @return std::string The URI of the request.
 */
std::string	Request::getUri(void) const {
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
            return (GET_METHOD);
        case POST:
            return (POST_METHOD);
        case HEAD:
            return (HEAD_METHOD);
        case PUT:
            return (PUT_METHOD);
        case DELETE:
            return (DELETE_METHOD);
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
    if (method == GET_METHOD)
        return (GET);
    else if (method == POST_METHOD)
        return (POST);
    else if (method == HEAD_METHOD)
        return (HEAD);
    else if (method == PUT_METHOD)
        return (PUT);
    else if (method == DELETE_METHOD)
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
bool	Request::validate(void) const {
    return (true); /// @todo: implement
}