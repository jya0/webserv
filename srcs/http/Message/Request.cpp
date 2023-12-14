/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:38:23 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/14 23:47:24 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<cstdlib>
#include	<sstream>
#include	"Request.hpp"
#include	"ServerSocket.hpp"
using namespace http;

/**
 * @brief Construct a new Request object (default constructor)
 */
Request::Request(void): AMessage() {
	_raw = tmpfile();
	return;
}

/**
 * @brief Construct a new Request object (copy constructor)
 *
 * @param RequestREF Request to copy
 */
Request::Request(Request const &RequestREF): AMessage() {
	this->Request::operator=(RequestREF);
	return;
}

/**
 * @brief Destroy the Request object
 */
Request::~Request(void) {
	if (_raw != NULL)
		fclose(_raw);
	return;
}

FILE	*http::duplicateFile(const FILE *input)
{
	FILE	*duplFile;
	char	*buffer;
	size_t	readReturn;

	buffer = new char[BUFFER_SIZE + 1];
	duplFile = NULL;
	if (input != NULL)
	{
		duplFile = tmpfile();
		if (duplFile == NULL)
			return (NULL);
		fseek(duplFile, 0, SEEK_SET);
		do {
			memset(buffer, 0, BUFFER_SIZE + 1);
			readReturn = fread(buffer, sizeof (char), BUFFER_SIZE, const_cast<FILE *>(input));
			fwrite(buffer, sizeof (char), readReturn, duplFile);
		} while (2);
		fseek(duplFile, 0, SEEK_SET);
	}
	delete [] (buffer);
	return (duplFile);
}

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
		if (requestREF.getRawData() != NULL)
		{
			fclose(_raw);
			_raw = duplicateFile(requestREF.getRawData());
		}
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

/**
 * @brief Construct a new Request object
 *
 * @param httpRaw The raw HTTP request to be parsed
 */
Request::Request(FILE *httpRaw): AMessage(httpRaw) {
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
		parseMessageBody();
	if (getHeaderValue("Content-Length") != "")
		_messageBody = _messageBody.substr(0, strtol(getHeaderValue("Content-Length").substr(0, 15 + 10).c_str(), NULL, 10));
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

/**
 * @brief Validates the headers of the request.
 *
 * @return true If the headers are valid.
 * @return false If the headers are invalid.
 */
bool Request::validate(void) const {
	return (true); /// @todo: implement
}

void Request::appendRawData(const std::string &bufSTR) {
	fseek(_raw, 0, SEEK_END);
	fwrite(bufSTR.c_str(), sizeof(char), bufSTR.size(), _raw);
	// _raw = _raw + _data;
}

bool Request::requestReady() const {
	return (_ready);
}
void Request::setRequestStatus(bool status) {
	_ready = status;
}

const FILE *Request::getRawData() const {
	return (_raw);
}

bool Request::recievedEOF() {

	return (false);
}
