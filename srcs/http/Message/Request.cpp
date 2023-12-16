/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:38:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/16 09:05:32 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<cstdlib>
#include	<sstream>
#include	<iostream>
#include	<unistd.h>
#include	<cstdio>
#include	"Request.hpp"
#include	"ServerSocket.hpp"
using namespace http;

/**
 * @brief Construct a new Request object (default constructor)
 */
Request::Request(void): AMessage() {
	_bodySize = 0;
}

/**
 * @brief Construct a new Request object (copy constructor)
 *
 * @param RequestREF Request to copy
 */
Request::Request(Request const &RequestREF): AMessage(RequestREF) {
	this->Request::operator=(RequestREF);
}

Request::Request(const std::string &messageHeader): AMessage(messageHeader) {
	_bodySize = 0;
}

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
		this->_bodySize = requestREF._bodySize;
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

void	http::printFile(FILE *file)
{
	char	*buffer;
	ssize_t	readReturn;

	if (file == NULL)
		return ;
	buffer = new char[MSG_BODY_BUFFER];
	fseek(file, 0, SEEK_SET);
	do {
		memset(buffer, 0, sizeof (char) * MSG_BODY_BUFFER);
		readReturn = fread(buffer, sizeof (char), MSG_BODY_BUFFER, file);
		std::cout << std::string(buffer, readReturn);
	} while (readReturn > 0);
	delete [](buffer);
	fseek(file, 0, SEEK_SET);
}


static size_t	getChunkSize(FILE *messageBody)
{
	int		c;
	int		index;
	char	chunkSizeBuf[HEX_STR_BUFFER + 1];

	index = 0;
	if (messageBody == NULL)
		return (0);
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
	ssize_t	bytesWrote;
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
			// char	c = fgetc(_messageBody);
			// c = fgetc(_messageBody);
			fseek(_messageBody, 2, SEEK_CUR);
			fgetpos(_messageBody, &readStart);
			fsetpos(_messageBody, &writeStart);
			bytesWrote = fwrite(chunkBuf, sizeof (char), bytesRead, _messageBody);
			fgetpos(_messageBody, &writeStart);
			totalLength += bytesWrote;
			delete [](chunkBuf);
		} while (chunkSize > 0);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	ftruncate(fileno(_messageBody), totalLength);
	std::cout << "LOGLOGLOGLOG" << http::getFileSize(_messageBody) << std::endl;
	// printFile(_messageBody);
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
	if (_messageBody == NULL)
		return ;
	fwrite(bufSTR.c_str(), sizeof(char), bufSTR.size(), _messageBody);
	_bodySize += bufSTR.size();
	// _messageBody = _messageBody + _data;
}

bool Request::requestReady() const {
	return (_ready);
}
void Request::setRequestStatus(bool status) {
	_ready = status;
}



bool http::rfind(FILE *haystack, std::string needle) {
	char		line[RFIND_RANGE];
	std::string lineStr;
	size_t		msgBodySize;
    bool        status;

	status = false;
	msgBodySize = http::getFileSize(haystack);
	if (msgBodySize == 0)
		return (true);
	fseek(haystack, -std::min(msgBodySize, size_t(RFIND_RANGE)), SEEK_END);
	memset(line, 0, RFIND_RANGE * sizeof(char));
	fread(line, sizeof(char), RFIND_RANGE, haystack);
	lineStr = std::string(line, RFIND_RANGE);
	if (lineStr.rfind(needle) != std::string::npos)
		status = true;
    fseek(haystack, 0, SEEK_SET);
	return (status);
}


bool Request::recievedLastByte() {
	size_t contentLength;
	
	contentLength = strtol(getHeaderValue("Content-Length").substr(0, 15 + 19).c_str(), NULL, 10);
	if (!getHeaderValue("content-length").empty())
		contentLength = strtol(getHeaderValue("content-length").substr(0, 15 + 19).c_str(), NULL, 10);
	return (this->_bodySize == contentLength);
}

bool Request::recievedEOF() {
	FILE	*file;
	fpos_t	checkPoint;
	bool	found;
	
	found = false;
	file = getMessageBody();
	if (file == NULL)
		return (true);
	fgetpos(_messageBody, &checkPoint);
    if (getHeaderValue("Transfer-Encoding") == "chunked" || getHeaderValue("transfer-encoding") == "chunked")
		found = http::rfind(file, "0\r\n\r\n");
	else if (!getHeaderValue("Content-Length").empty() || !getHeaderValue("content-length").empty())
		found = recievedLastByte();
	else
		found = true; //no body
	fsetpos(_messageBody, &checkPoint);
	return (found);
}
