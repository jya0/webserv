/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messge.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:43:29 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 18:10:33 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"AMessage.hpp"
#include    <sstream>

AMessage::AMessage(void) {
    return ;
}

AMessage::AMessage(AMessage &aMessageREF) {
    *this = aMessageREF;
    return ;
}

AMessage::~AMessage(void) {
    return ;
}

AMessage	&AMessage::operator=(AMessage &aMessageREF) {
    if (this != &aMessageREF) {
        this->_startLine = aMessageREF._startLine;
        this->_headers = aMessageREF._headers;
        this->_messageBody = aMessageREF._messageBody;
    }
    return (*this);
}

AMessage::AMessage(std::string startLine, std::list<std::pair<std::string,
        std::string>> headers, std::string messageBody) {
    this->_startLine = startLine;
    this->_headers = headers;
    this->_messageBody = messageBody;
    return ;
}

AMessage::AMessage(std::string rawMessage) {

    std::istringstream iss(rawMessage);
    std::string line;
    while(std::getline(iss, line, '\r')) {
        iss.ignore();
        if (this->_startLine == "")
            this->_startLine = line;
        else if (line == "")
            break ;
        else {
            std::string key;
            std::string value;
            std::istringstream iss2(line);
            std::getline(iss2, key, ':');
            std::getline(iss2, value, '\r');
            this->_headers.push_back(std::make_pair(key, value));
        }
    }
    this->_messageBody = iss.str().substr(iss.tellg());
}