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

Request::Request(std::string httpRaw) {
    // AMessage(httpRaw); @note: this is not working, how would I call the
    // parent constructor?
    this->_httpMethod = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_uri = this->_startLine.substr(this->_startLine.find(' ') + 1,
            this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
            this->_startLine.find(' ') - 1);
    this->_httpVersion = this->_startLine.substr(this->_startLine.find(' ',
                this->_startLine.find(' ') + 1) + 1);
}

std::string	Request::getHttpMethod(void) {
    return (this->_httpMethod);
}

std::string	Request::getHttpVersion(void) {
    return (this->_httpVersion);
}

std::string	Request::getUri(void) {
    return (this->_uri);
}