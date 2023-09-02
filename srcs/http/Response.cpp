/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:30:42 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 11:12:18 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Response.hpp"

Response::Response(void) {
    return ;
}

Response::Response(Response &responseREF) {
    *this = responseREF;
    return ;
}

Response::~Response(void) {
    return ;
}

Response	&Response::operator=(Response &responseREF) {
    if (this != &responseREF) {
        this->_httpVersion = responseREF._httpVersion;
        this->_httpStatusCode = responseREF._httpStatusCode;
    }
    return (*this);
}

Response::Response(std::string httpRaw): AMessage(httpRaw) {
    this->_httpVersion = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_httpStatusCode = std::stoi(this->_startLine.substr(this->_startLine.find(' ') + 1,
                this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
                this->_startLine.find(' ') - 1));
}

std::string	Response::getHttpVersion(void) {
    return (this->_httpVersion);
}

unsigned short	Response::getHttpStatusCode(void) {
    return (this->_httpStatusCode);
}