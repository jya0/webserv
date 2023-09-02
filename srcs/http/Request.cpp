/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:44:11 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 17:55:52 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Request.hpp"

Request::Request(void) {
    return ;
}

Request::Request(Request const &RequestREF) {
    *this = RequestREF;
    return ;
}

Request::~Request(void) {
    return ;
}

Request	&Request::operator=(Request const &RequestREF) {
    if (this != &RequestREF) {
        this->_httpMethod = RequestREF._httpMethod;
        this->_uri = RequestREF._uri;
    }
    return (*this);
}

Request::Request(std::string httpRaw): AMessage(httpRaw) {
    std::string method = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_httpMethod = this->methodEnum(method);
    this->_uri = this->_startLine.substr(this->_startLine.find(' ') + 1,
            this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
            this->_startLine.find(' ') - 1);
    this->_httpVersion = this->_startLine.substr(this->_startLine.find(' ',
                this->_startLine.find(' ') + 1) + 1);
}

std::string	Request::getHttpMethod(void) {
    return (Request::methodName(this->_httpMethod));
}

std::string	Request::getHttpVersion(void) {
    return (this->_httpVersion);
}

std::string	Request::getUri(void) {
    return (this->_uri);
}

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