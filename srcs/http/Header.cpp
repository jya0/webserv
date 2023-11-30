/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:19:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/11/28 21:08:45 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Header.hpp"

using namespace http;

Header::Header(): _name(""), _value("") {}

Header::Header(std::string name, std::string value):
    _name(name), _value(value) {}

Header::Header(Header const & src) {
    *this = src;
}

Header::~Header() {}

Header & Header::operator=(Header const & src) {
    if (this != &src) {
        this->_name = src._name;
        this->_value = src._value;
    }
    return *this;
}

std::string Header::toString(void) const {
    return this->_name + ": " + this->_value;
}

std::string Header::getName(void) const {
    return this->_name;
}

std::string Header::getValue(void) const {
    return this->_value;
}
