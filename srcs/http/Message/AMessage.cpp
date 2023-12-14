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

#include <sstream>
#include <algorithm>
#include "AMessage.hpp"
#include "ToString.tpp"
#include "ServerParser_namespace.hpp"
using namespace http;

/**
 * @brief Construct a new AMessage object (default constructor)
 */
AMessage::AMessage(void)
{
	return;
}

/**
 * @brief Construct a new AMessage object (copy constructor)
 *
 * @param aMessageREF Message to copy
 */
AMessage::AMessage(const AMessage &aMessageREF)
{
	*this = aMessageREF;
	return;
}

/**
 * @brief Destroy the AMessage object
 */
AMessage::~AMessage(void)
{
	return;
}

/**
 * @brief The copy assignment operator of the AMessage class.
 *
 * @param aMessageREF Message to copy
 * @return AMessage& Reference to the copied message
 */
AMessage &AMessage::operator=(const AMessage &aMessageREF)
{
	if (this != &aMessageREF)
	{
		this->_startLine = aMessageREF._startLine;
		this->_headers = aMessageREF._headers;
		this->_messageBody = aMessageREF._messageBody;
		this->_messageBodySize = aMessageREF._messageBodySize;
	}
	return (*this);
}

/**
 * @brief Construct a new AMessage object
 *
 * @param startLine The first line of the message
 * @param headers The list of headers of the message
 * @param messageBody The body of the message
 */
AMessage::AMessage(std::string startLine, std::list<Header> headers, std::string messageBody, size_t messageBodySize)
{
	this->_startLine = startLine;
	this->_headers = headers;
	this->_messageBody = messageBody;
	this->_messageBodySize = messageBodySize;
	return;
}

/* void AMessage::parseMessageBody()
{
	std::string head = _messageBody.substr(0, _messageBody.find("\r\n"));
	if (head.empty())
		return ;
	std::string chunks = _messageBody.substr(_messageBody.find("\r\n") + 2, _messageBody.size() - 1);
	std::string subchunk = chunks.substr(0, 20);
	std::string body = "";
	int chunksize = strtol(head.c_str(), NULL, 16);
	size_t i = 0;

	while (chunksize)
	{
		body += chunks.substr(i, chunksize);
		// i = chunks.find("\r\n", i) + 2;
		i += chunksize + 2;
		subchunk = chunks.substr(i, 20);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
	// _messageBody = head + "\r\n\r\n" + body + "\r\n\r\n";
	_messageBody = body;
} */

void AMessage::parseMessageBody()
{
	// std::string								result;
	// size_t									chunkSize;
	// std::pair< std::string, std::string >	sizeNchunk;

	// do {
	// 	sizeNchunk = ServerParser::splitByTwo(_messageBody, '\r');
	// 	sizeNchunk.second.erase(sizeNchunk.second.begin());
	// 	chunkSize = std::strtol(sizeNchunk.first.c_str(), NULL, 16);
	// 	result += sizeNchunk.second.substr(0, chunkSize);
	// 	_messageBody = _messageBody.substr(chunkSize + std::string(CR_LF).size() + 1, std::string::npos);
	// } while (_messageBody.size());
	// _messageBody = result;
}

/**
 *
 * @brief Construct a new AMessage object
 *
 * @param rawMessage The raw message to be parsed
 */
AMessage::AMessage(std::string rawMessage) : _messageBodySize(rawMessage.size())
{
	std::istringstream iss(rawMessage);
	std::string line;

	while (std::getline(iss, line, '\r'))
	{
		iss.ignore();
		if (this->_startLine == "")
			this->_startLine = line;
		else if (line == "")
			break;
		else
		{
			std::string key;
			std::string value;
			std::istringstream iss2(line);
			std::getline(iss2, key, ':');
			std::getline(iss2, value, '\r');
			value = value.substr(1, value.size());
			this->_headers.push_back(Header(key, value));
		}
	}
	this->_messageBody = iss.str().substr(iss.tellg());
}

/**
 * @brief Returns the start line of the message.
 *
 * @return std::string The start line of the message
 */
std::string AMessage::getStartLine(void)
{
	return (this->_startLine);
}

/**
 * @brief Returns the headers of the message.
 *
 * @return std::list<Header> A list containing the headers of the message
 */
std::list<Header> AMessage::getHeaders(void) const
{
	return (this->_headers);
}

/**
 * @brief Returns the value of a given header key
 *
 * @return std::string the header value if it finds anything
 */
std::string AMessage::getHeaderValue(const std::string &headerKey) const
{
	std::list<Header>::const_iterator itc;

	itc = std::find_if(_headers.begin(), _headers.end(), Header::IsHeaderKeyUnary(headerKey));
	if (itc == _headers.end())
		return ("");
	return (itc->getValue());
};

/**
 * @brief Returns the body of the message.
 *
 * @return std::string The body of the message
 */
std::string AMessage::getMessageBody(void) const
{
	return (this->_messageBody);
}

/**
 * @brief Converts the message to a string.
 *
 * @return std::string The raw message
 */
std::string AMessage::getRawMessage(void) const
{
	std::string rawMessage = this->_startLine;
	for (std::list<Header>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		if (rawMessage.size() == 0)
			rawMessage = it->getKey() + ": " + it->getValue() + CR_LF;
		else
			rawMessage += it->getKey() + ": " + it->getValue() + CR_LF;
	}
	rawMessage += CR_LF + this->_messageBody;
	// if (_messageBody.find(CR_LF CR_LF) == std::string::npos)
	// 	rawMessage += CR_LF CR_LF;
	return (rawMessage);
}

void AMessage::addHeader(Header header)
{
	std::list<Header>::iterator it;

	it = std::find_if(_headers.begin(), _headers.end(), Header::IsHeaderKeyUnary(header.getKey()));
	if (it != _headers.end())
		it->setValue(header.getValue());
	else
		this->_headers.push_back(header);
}

void AMessage::setMessageBody(std::string messageBody)
{
	this->_messageBody = messageBody;
	addHeader(Header("Content-Length", http::toString(_messageBody.size())));
}

void AMessage::setStartLine(std::string startLine)
{
	this->_startLine = startLine;
}