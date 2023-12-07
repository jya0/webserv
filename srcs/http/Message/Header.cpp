/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:19:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/07 22:05:01 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Header.hpp"
#include	<iostream>

using namespace http;

Header::Header(): _key(""), _value("") {}

Header::Header(std::string name, std::string value):
    _key(name), _value(value) {}

Header::Header(Header const & src) {
    *this = src;
}

Header::~Header() {}

Header & Header::operator=(Header const & src) {
    if (this != &src) {
        this->_key = src._key;
        this->_value = src._value;
    }
    return *this;
}

std::string Header::toString(void) const {
    return this->_key + ": " + this->_value;
}

std::string Header::getKey(void) const {
    return this->_key;
}

std::string Header::getValue(void) const {
    return this->_value;
}

void		Header::setKey(const std::string &keyREF) {
	_key = keyREF;
};

void		Header::setValue(const std::string &valueREF) {
	_value = valueREF;
};

Header::IsHeaderKeyUnary::IsHeaderKeyUnary(const std::string &headerKeyToFind): _headerKeyToFind(headerKeyToFind) {};

bool	Header::IsHeaderKeyUnary::operator()(const Header &headerREF)
{
	return (headerREF.getKey() == _headerKeyToFind);
};
