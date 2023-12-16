/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messge.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.MSG_BODY_BUFFERabudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:43:29 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 18:10:33 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<sstream>
#include	<algorithm>
#include	<cstdio>
#include	<cstring>
#include	<unistd.h>
#include	"AMessage.hpp"
#include	"ToString.tpp"
#include	"ServerParser_namespace.hpp"
#include	"ServerSocket.hpp"
#include	<sys/stat.h>

using namespace http;

/**
 * @brief Construct a new AMessage object (default constructor)
 */
AMessage::AMessage(void): _ready(false)
{
	_messageBody = tmpfile();
	if (_messageBody == NULL)
		std::cerr << "<<<<<<<<<<<<<<<<<<<<<NO TEMP FILE!" << std::endl;
}

/**
 * @brief Construct a new AMessage object (copy constructor)
 *
 * @param aMessageREF Message to copy
 */
AMessage::AMessage(const AMessage &aMessageREF): _messageBody(NULL)
{
	*this = aMessageREF;
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
		_startLine = aMessageREF._startLine;
		_headers = aMessageREF._headers;
		if (_messageBody != NULL)
		{
			close(fileno(_messageBody));
			fclose(_messageBody);
		}
		_messageBody = duplicateFile(aMessageREF._messageBody);
		_httpVersion = aMessageREF._httpVersion;
		_ready = aMessageREF._ready;
	}
	return (*this);
}

/**
 *
 * @brief Construct a new AMessage object
 *
 * @param rawMessage The raw message to be parsed
 */
AMessage::AMessage(std::string messageHeader):  _ready(false), _messageBody(NULL)
{
	std::istringstream iss(messageHeader);
	std::string line;

	_messageBody = tmpfile();
	if (_messageBody == NULL)
		std::cerr << "<<<<<<<<<<<<<<<<<<<<<NO TEMP FILE!" << std::endl;
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
			if (!value.empty())
				value = value.substr(1, value.size());
			this->_headers.push_back(Header(key, value));
		}
	}
}


/**
 * @brief Destroy the AMessage object
 */
AMessage::~AMessage(void)
{
	if (_messageBody != NULL)
	{
		close(fileno(_messageBody));
		fclose(_messageBody);
	}
	return;
}

FILE	*http::duplicateFile(const FILE *input)
{
	FILE	*duplFile;
	char	*buffer;
	size_t	readReturn;

	buffer = new char[MSG_BODY_BUFFER];
	duplFile = NULL;
	if (input != NULL)
	{
		duplFile = tmpfile();
		if (duplFile == NULL)
			return (NULL);
		fseek(duplFile, 0, SEEK_SET);
		fseek(const_cast<FILE *>(input), 0, SEEK_SET);
		do {
			memset(buffer, 0, MSG_BODY_BUFFER);
			readReturn = fread(buffer, sizeof (char), MSG_BODY_BUFFER, const_cast<FILE *>(input));
			fwrite(buffer, sizeof (char), readReturn, duplFile);
		} while (readReturn > 0);
		fseek(duplFile, 0, SEEK_SET);
	}
	delete [] (buffer);
	return (duplFile);
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
FILE	*AMessage::getMessageBody(void) const
{
	return (this->_messageBody);
}

std::string	AMessage::getMessageBodyStr(void) const
{
	return (fileToString(_messageBody));
}

std::string	http::fileToString(FILE *file)
{
	char		line[MSG_BODY_BUFFER];
	size_t		bytesRead;
	std::string msgBody;

	bytesRead = 0;
	fseek(file, 0, SEEK_SET);
	do {
		memset(line, 0, MSG_BODY_BUFFER * sizeof(char));
		bytesRead = fread(line, sizeof(char), MSG_BODY_BUFFER, file);
		msgBody += std::string(line, MSG_BODY_BUFFER);
	} while (bytesRead > 0);
	fseek(file, 0, SEEK_SET);
	return (msgBody);
}

size_t http::getFileSize(FILE *file)
{
	struct stat stat_buf;
	int			status;
	if (!file)
		return (0);
	status = fstat(fileno(file), &stat_buf);
	if (status == 0)
		return (stat_buf.st_size);
	return (0);
}

std::string	AMessage::getStartAndHeader(void) const {
	std::string	startNheader;

	startNheader = _startLine;
	for (std::list< Header >::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		startNheader += it->getKey() + ": " + it->getValue() + CR_LF;
	startNheader += CR_LF;
	return (startNheader);
};

/**
 * @brief Converts the message to a string.
 *
 * @return std::string The raw message
 */
t_raw_message	AMessage::getRawMessage(void) const
{
	return (std::make_pair< std::string, FILE * >(getStartAndHeader(), _messageBody));
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

void AMessage::setMessageBody(const std::string &msgBodyREF)
{
	if (_messageBody == NULL)
		_messageBody = tmpfile();
	fseek(_messageBody, 0, SEEK_SET);
	ftruncate(fileno(_messageBody), 0);
	fwrite(msgBodyREF.c_str(), sizeof (char), msgBodyREF.size(), _messageBody);
	addHeader(Header("Content-Length", http::toString(msgBodyREF.size())));
	fseek(_messageBody, 0, SEEK_SET);
}

void AMessage::setMessageBody(FILE *msgBody)
{
	if (msgBody == NULL)
		return ;
	if (_messageBody != NULL)
	{
		close(fileno(_messageBody));
		fclose(_messageBody);
	}
	_messageBody = msgBody;
	addHeader(Header("Content-Length", http::toString(http::getFileSize(msgBody))));
	fseek(_messageBody, 0, SEEK_SET);
}

void	http::filecpy(FILE *src, FILE *dst)
{
	char	*buffer;
	ssize_t	bytesRead;
	ssize_t totalWritten = 0;
	ssize_t totalToWrite = 0;
	if (src == NULL || dst == NULL)
		return ;
	buffer = new char[MSG_BODY_BUFFER];
	fseek(src, 0, SEEK_SET);
	fseek(dst, 0, SEEK_SET);
	do {
		memset(buffer, 0, sizeof (char) * MSG_BODY_BUFFER);
		bytesRead = fread(buffer, sizeof (char), MSG_BODY_BUFFER, src);
		totalToWrite += bytesRead;
		totalWritten += fwrite(buffer, sizeof (char), bytesRead, dst);
	} while (bytesRead > 0);
	delete [](buffer);
	std::cout << "SRCS: " << http::getFileSize(src) << std::endl;
	std::cout << "DST: " << http::getFileSize(dst) << std::endl;
	fseek(src, 0, SEEK_SET);
	fseek(dst, 0, SEEK_SET);
}

void	AMessage::loadFileToMessageBody(const std::string &filePathREF)
{
	FILE	*infile;

	if (_messageBody == NULL)
		_messageBody = tmpfile();
	fseek(_messageBody, 0, SEEK_SET);
	infile = fopen(filePathREF.c_str(), "r");
	filecpy(infile, _messageBody);
	close(fileno(infile));
	fclose(infile);
	fseek(_messageBody, 0, SEEK_SET);
	addHeader(Header("Content-Length", http::toString(http::getFileSize(_messageBody))));
}

void AMessage::setStartLine(std::string startLine)
{
	this->_startLine = startLine;
}