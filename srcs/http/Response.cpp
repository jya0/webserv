/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:30:42 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 15:12:05 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Http_namespace.hpp"

using namespace http;

/**
 * @brief Construct a new Response object (default constructor)
 */
Response::Response(void) {
    return ;
}

/**
 * @brief Construct a new Response object from a status code.
 * 
 * @param status The status code of the response
 */
Response::Response(int status) {
    this->_httpVersion = "HTTP/1.1";
    this->_httpStatusCode = status;
    return ;
}
/**
 * @brief Construct a new Response object (copy constructor)
 * 
 * @param responseREF Response object to copy
 */
Response::Response(Response &responseREF) {
    *this = responseREF;
    return ;
}

/**
 * @brief Destroy the Response object
 */
Response::~Response(void) {
    return ;
}

/**
 * @brief The Response copy assignment operator.
 * 
 * @param responseREF Response object to copy
 * @return Response& Reference to the new Response object
 */
Response	&Response::operator=(Response &responseREF) {
    if (this != &responseREF) {
        this->_httpVersion = responseREF._httpVersion;
        this->_httpStatusCode = responseREF._httpStatusCode;
    }
    return (*this);
}

/**
 * @brief Construct a new Response object from a string.
 * 
 * @param httpRaw The string to parse
 */
Response::Response(std::string httpRaw): AMessage(httpRaw) {
    this->_httpVersion = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_httpStatusCode = std::stoi(this->_startLine.substr(this->_startLine.find(' ') + 1,
                this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
                this->_startLine.find(' ') - 1));
}

/**
 * @brief Return the HTTP version of the response.
 * 
 * @return std::string The HTTP version of the response 
 */
std::string	Response::getHttpVersion(void) {
    return (this->_httpVersion);
}

/**
 * @brief Return the HTTP status code of the response.
 * 
 * @return unsigned short The HTTP status code of the response
 */
unsigned short	Response::getHttpStatusCode(void) {
    return (this->_httpStatusCode);
}