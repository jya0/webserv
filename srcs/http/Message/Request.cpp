/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:38:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/15 05:34:57 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<cstdlib>
#include	<sstream>
#include	<iostream>
#include	<unistd.h>
#include	<stdio.h>
#include	"Request.hpp"
#include	"ServerSocket.hpp"
using namespace http;

/**
 * @brief Construct a new Request object (default constructor)
 */
Request::Request(void): AMessage() {}

/**
 * @brief Construct a new Request object (copy constructor)
 *
 * @param RequestREF Request to copy
 */
Request::Request(Request const &RequestREF): AMessage() {
	this->Request::operator=(RequestREF);
}

Request::Request(const std::string &messageHeader): AMessage(messageHeader) {}

/**
 * @brief Destroy the Request object
 */
Request::~Request(void) {}

/**
 * @brief The copy assignment operator of the Request class.
 *
 * @param RequestREF Request to copy
 * @return Request& Reference to the copied Request
 */
Request &Request::operator=(Request const &requestREF) {
	if (this != &requestREF)
	{
		this->AMessage::operator=(requestREF);
		this->_httpMethod = requestREF._httpMethod;
		this->_uri = requestREF._uri;
	}
	return (*this);
}

static std::string	decodeUri(const std::string &encoded)
{
	std::ostringstream	decoded;
	int					hexValue;
	std::istringstream	hexStream;

    for (std::size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            hexStream.str(encoded.substr(i + 1, 2));
            hexStream >> std::hex >> hexValue;
            decoded << static_cast<char>(hexValue);
            i += 2;
        } else {
            decoded << encoded[i];
        }
    }
    return (decoded.str());
};


// {
// 	do {
// 		sizeNchunk = ServerParser::splitByTwo(_messageBody, '\r');
// 		sizeNchunk.second.erase(sizeNchunk.second.begin());
// 		chunkSize = std::strtol(sizeNchunk.first.c_str(), NULL, 16);
// 		result += sizeNchunk.second.substr(0, chunkSize);
// 		_messageBody = _messageBody.substr(chunkSize + std::string(CR_LF).size() + 1, std::string::npos);
// 	} while (_messageBody.size());
// 	_messageBody = result;
// }

static size_t	getChunkSize(FILE *messageBody)
{
	int		c;
	int		index;
	char	chunkSizeBuf[HEX_STR_BUFFER + 1];

	index = 0;
	memset(chunkSizeBuf, 0, sizeof (char) * (HEX_STR_BUFFER + 1));
	do {
		c = fgetc(messageBody);
		chunkSizeBuf[index % HEX_STR_BUFFER] = c;
		++index;
	} while (c != CR);
	fseek(messageBody, 1, SEEK_CUR); // to skip '\n'
	return (std::strtol(chunkSizeBuf, NULL, 16));
}

void	Request::parseChunked(void)
{
	char	*chunkBuf;
	size_t	chunkSize;
	size_t	totalLength;
	ssize_t	bytesRead;
	fpos_t	writeStart;
	fpos_t	readStart;

	totalLength = 0;
	fseek(_messageBody, 0, SEEK_SET);
	fgetpos(_messageBody, &readStart);
	fgetpos(_messageBody, &writeStart);
	try {
		do {
			fsetpos(_messageBody, &readStart);
			chunkSize = getChunkSize(_messageBody);
			chunkBuf = new char[chunkSize];
			memset(chunkBuf, 0, sizeof (char) * chunkSize);
			bytesRead = fread(chunkBuf, sizeof (char), chunkSize, _messageBody);
			totalLength += bytesRead;
			fgetpos(_messageBody, &readStart);
			fsetpos(_messageBody, &writeStart);
			fwrite(chunkBuf, sizeof (char), chunkSize, _messageBody);
			fgetpos(_messageBody, &writeStart);
			delete [](chunkBuf);
		} while (chunkSize > 0);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	ftruncate(fileno(_messageBody), totalLength);
	fseek(_messageBody, 0, SEEK_SET);
}

void	Request::parseRequest(void)
{
	std::string method = _startLine.substr(0, _startLine.find(' '));
	_httpMethod = methodEnum(method);
	_uri = _startLine.substr(_startLine.find(' ') + 1,
								_startLine.find(' ',
									_startLine.find(' ') + 1) -
										_startLine.find(' ') - 1);
	_uri = decodeUri(_uri);
	_httpVersion = _startLine.substr(_startLine.find(' ',
										_startLine.find(' ') + 1) + 1);
	if (getHeaderValue("Transfer-Encoding") == "chunked")
		parseChunked();
// 	if (getHeaderValue("Content-Length") != "")
// 		_messageBody = _messageBody.substr(0, strtol(getHeaderValue("Content-Length").substr(0, 15 + 10).c_str(), NULL, 10));
}

/**
 * @brief Returns the HTTP method of the request.
 *
 * @return std::string The HTTP method of the request.
 */
std::string Request::getHttpMethod(void) const {
	return (Request::methodName(_httpMethod));
}

const e_httpMethod &Request::getHttpMethodEnum(void) const {
	return (_httpMethod);
};

/**
 * @brief Returns the HTTP version of the request.
 *
 * @return std::string The HTTP version of the request.
 */
const std::string &Request::getHttpVersion(void) const {
	return (_httpVersion);
}

/**
 * @brief Returns the URI of the request.
 *
 * @return std::string The URI of the request.
 */
const std::string &Request::getUri(void) const {
	return (this->_uri);
}

/**
 * @brief Swaps out the enum for the name of the HTTP method.
 *
 * @param method The enum of the HTTP method.
 * @return std::string The name of the HTTP method.
 */
std::string Request::methodName(e_httpMethod method) {
	switch (method)
	{
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
		return (NO_METHOD);
}

void Request::appendRawData(const std::string &bufSTR) {
	fseek(_messageBody, 0, SEEK_END);
	fwrite(bufSTR.c_str(), sizeof(char), bufSTR.size(), _messageBody);
	_messageBodySize += bufSTR.size();
	fseek(_messageBody, 0, SEEK_SET);

	// _messageBody = _messageBody + _data;
}

bool Request::requestReady() const {
	return (_ready);
}
void Request::setRequestStatus(bool status) {
	_ready = status;
}

bool Request::recievedLastChunk() {
	char		line[42];
	size_t		bytesRead;
	std::string lineStr;

	bytesRead = 0;
	fseek(_messageBody, -1, SEEK_END); // seek to end of file
	for (int i = _messageBodySize; i > 0; i-= 42)
	{
		memset(line, 0, 42 * sizeof(char));
		bytesRead += fread(line, sizeof(char), 42, _messageBody); // This progresses the seek location
		lineStr = std::string(line);
		if (lineStr.rfind("\r\n"))
			return (true);
		if (bytesRead == _messageBodySize)
			break;
		fseek(_messageBody, -43, SEEK_CUR); // Go back 2 bytes (1 to compensate)
	}
	return (false);
}

bool Request::recievedLastByte() {
	size_t contentLength = strtol(getHeaderValue("Content-Length").substr(0, 15 + 10).c_str(), NULL, 10);
	return (_messageBodySize >= contentLength);
}

bool Request::recievedEOF() {
	if (getHeaderValue("Transfer-Encoding") == "chunked")
		return (recievedLastChunk());
	if (getHeaderValue("Content-Length") != "")
		return (recievedLastByte());
	return (false);
}
