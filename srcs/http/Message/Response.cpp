/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:30:42 by jyao              #+#    #+#             */
/*   Updated: 2023/12/16 05:50:47 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<fstream>
#include	<sstream>
#include	<cstdlib>
#include	"ServerParser_namespace.hpp"
#include 	"AMessage.hpp"
#include	"Request.hpp"
#include	"Response.hpp"
#include	"Header.hpp"
#include	"CGIhandler.hpp"
#include	"Autoindex_namespace.hpp"
#include	"MimeTypes.hpp"
#include	"ToString.tpp"

using namespace http;

/**
 * @brief Construct a new Response object (default constructor)
 */
Response::Response(void) : AMessage()
{
	this->addHeader(Header("Content-Type", "text/html"));
	this->addHeader(Header("Server", SERVER_SOFTWARE));
	_ready = false;
	_packetStartPos = 0;
	_packetStatus = NOT_STARTED;
	return ;
}

/**
 * @brief Construct a new Response object from a status code.
 *
 * @param status The status code of the response
 */
Response::Response(int status) : AMessage()
{
	std::stringstream	ss;

	this->_httpVersion = "HTTP/1.1";
	this->_httpStatusCode = status;
	ss << this->_httpVersion << " " << status << " " << this->getHttpStatusString(status) << CR_LF;
	this->_startLine = ss.str();
	this->setMessageBody(this->getHttpStatusString(status));
	_ready = true;
	_packetStartPos = 0;
	_packetStatus = NOT_STARTED;
	return;
}

/**
 * @brief Construct a new Response object from status code and specified body
 *
 * @param status
 * @param responseBody
 */
Response::Response(int status, const std::string &responseBody)
{
	*this = Response(status);
	this->setMessageBody(responseBody);
};

/**
 * @brief Construct a new Response object (copy constructor)
 *
 * @param responseREF Response object to copy
 */
Response::Response(const Response &responseREF): AMessage(responseREF)
{
	*this = responseREF;
	return;
}

/**
 * @brief Destroy the Response object
 */
Response::~Response(void) {}

/**
 * @brief The Response copy assignment operator.
 *
 * @param responseREF Response object to copy
 * @return Response& Reference to the new Response object
 */
Response &Response::operator=(const Response &responseREF)
{
	if (this != &responseREF)
	{
		AMessage::operator=(responseREF);
		this->_httpVersion = responseREF._httpVersion;
		this->_httpStatusCode = responseREF._httpStatusCode;
		this->_ready = responseREF._ready;
		this->_packetStartPos = responseREF._packetStartPos;
		this->_packetStatus = responseREF._packetStatus;
	}
	return (*this);
}

/**
 * @brief Construct a new Response object from a string.
 *
 * @param httpRaw The string to parse
 */
Response::Response(const std::string &messageHeader): AMessage(messageHeader)
{
	_httpVersion		=	this->_startLine.substr(0, this->_startLine.find(' '));
	_httpStatusCode		=	strtol(this->_startLine.substr(this->_startLine.find(' ') + 1,
								this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
								this->_startLine.find(' ') - 1).c_str(), NULL, 10);
	_ready = true;
	_packetStartPos = 0;
	_packetStatus = NOT_STARTED;
}

int Response::getPacketStatus() const
{
	return (_packetStatus);
}

void Response::setPacketStatus(int _status)
{
	_packetStatus = _status;
}

int Response::getPacketStartPos() const
{
	return (_packetStartPos);
}

void Response::movePacketStartPos(int newPos)
{
	_packetStartPos = newPos;
}


ErrorPageResponse::ErrorPageResponse(void): Response() {};

ErrorPageResponse::~ErrorPageResponse(void) {};

ErrorPageResponse::ErrorPageResponse(const ErrorPageResponse &eprREF): Response(eprREF) {};

ErrorPageResponse	&ErrorPageResponse::operator=(const ErrorPageResponse &eprREF) {
	this->Response::operator=(eprREF);
	return (*this);
};

ErrorPageResponse::ErrorPageResponse(const int &status, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR): Response(status) {
	std::string	epFile;
	std::string	root;
	std::string	epFilePath;
	std::vector<ServerConfig::ErrorPage> errorPages;

	if (locPTR != NULL && !locPTR->getErrorPages().empty())
		epFile = locPTR->getErrorPage(status);
	else if (!servConfREF.getErrorPages().empty())
		epFile = servConfREF.getErrorPage(status);
	root = servConfREF.getRoot();
	if (locPTR != NULL && !locPTR->getRoot().empty())
		root = locPTR->getRoot();
	epFilePath = root + epFile;
	if (Autoindex::isPathExist(epFilePath) > 0 && Autoindex::isPathReg(epFilePath) > 0)
		loadFileToMessageBody(epFilePath);
};

RedirectResponse::RedirectResponse(void): Response() {};

RedirectResponse::~RedirectResponse(void) {};

RedirectResponse::RedirectResponse(const RedirectResponse &rrREF): Response(rrREF) {};

RedirectResponse	&RedirectResponse::operator=(const RedirectResponse &rrREF) {
	this->Response::operator=(rrREF);
	return (*this);
};

RedirectResponse::RedirectResponse(const ServerConfig &servConfREF, const ServerConfig::Location *locPTR) {
	ServerConfig::Return	returnObj;

	if (locPTR != NULL)
		returnObj = locPTR->getReturn();
	if (!returnObj.isInit)
		returnObj = servConfREF.getReturn();
	if (returnObj.isInit)
	{
		Response	redirect(returnObj.code);

		redirect.addHeader(Header("Location", returnObj.uri));
		this->Response::operator=(redirect);
	}
};

/**
 * @brief Return the HTTP version of the response.
 *
 * @return std::string The HTTP version of the response
 */
std::string Response::getHttpVersion(void) const
{
	return (this->_httpVersion);
}

/**
 * @brief Return the HTTP status code of the response.
 *
 * @return unsigned short The HTTP status code of the response
 */
unsigned short Response::getHttpStatusCode(void) const
{
	return (this->_httpStatusCode);
}

bool Response::responseReady() const
{
	return (_ready);
}

void Response::setResponseStatus(bool status)
{
	_ready = status;
}

/**
 * @brief Return the HTTP status string of the response.
 *
 * @return std::string The HTTP status string of the response
 */
bool Response::validate(void) const
{
	return (true);
}

/*
██╗  ██╗████████╗████████╗██████╗     ███╗   ███╗███████╗████████╗██╗  ██╗ ██████╗ ██████╗ ███████╗
██║  ██║╚══██╔══╝╚══██╔══╝██╔══██╗    ████╗ ████║██╔════╝╚══██╔══╝██║  ██║██╔═══██╗██╔══██╗██╔════╝
███████║   ██║      ██║   ██████╔╝    ██╔████╔██║█████╗     ██║   ███████║██║   ██║██║  ██║███████╗
██╔══██║   ██║      ██║   ██╔═══╝     ██║╚██╔╝██║██╔══╝     ██║   ██╔══██║██║   ██║██║  ██║╚════██║
██║  ██║   ██║      ██║   ██║         ██║ ╚═╝ ██║███████╗   ██║   ██║  ██║╚██████╔╝██████╔╝███████║
╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝         ╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝
*/

static std::string getFilePath(const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	std::string filePath;
	std::pair<std::string, std::string> uriNquery;

	if (locPTR == NULL)
		return ("");
	filePath = locPTR->getRoot();
	if (filePath.empty())
		filePath = servConfREF.getRoot();
	uriNquery = ServerParser::splitByTwo(requestREF.getUri(), QUERY_DELIM);
	if (checkMimeType(uriNquery.first) == MIME_CGI)
		filePath += uriNquery.first.substr(1, std::string::npos);
	else
		filePath += uriNquery.first.substr(locPTR->locationUri.size(), std::string::npos);
	return (filePath);
}

static void setHeader(Response &response, const std::string &fileStr)
{
	std::string	mimeType;

	(void)response;
	mimeType = http::checkMimeType(fileStr);
	if (mimeType.empty())
		mimeType = "text/html";
	std::cerr << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << mimeType << std::endl;
	response.addHeader(Header("Content-Type", mimeType));
	response.addHeader(Header("Content-Length", http::toString(http::getFileSize(response.getMessageBody()))));
}

void	loadIndex(Response &responseREF, const std::vector< std::string > &indexPages, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	std::string	indexPagePath;
	std::string	root;

	root = servConfREF.getRoot();
	if (!locPTR->getRoot().empty())
		root = locPTR->getRoot();
	for (std::vector< std::string >::const_iterator itc = indexPages.begin(); itc != indexPages.end(); ++itc)
	{
		indexPagePath = root + *itc;
		if (Autoindex::isPathExist(indexPagePath) > 0 && Autoindex::isPathReg(indexPagePath) > 0)
			responseREF.loadFileToMessageBody(indexPagePath);
	}
}

static Response loadContent(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	Response					response(200);
	std::string					result;
	std::vector< std::string >	indexPages;

	if (locPTR == NULL)
		throw (404);
	if (Autoindex::isPathFolder(filePathREF) > 0)
	{
		indexPages = locPTR->getIndex();
		indexPages.insert(indexPages.end(), servConfREF.getIndex().begin(), servConfREF.getIndex().end());
		if (locPTR->getAutoIndex() == true)
			response.setMessageBody(Autoindex::genPage(filePathREF, requestREF));
		else if (!indexPages.empty() && requestREF.getUri() == locPTR->locationUri)
			loadIndex(response, indexPages, servConfREF, locPTR);
		else
			throw (404); // forbidden
	}
	else if (Autoindex::isPathReg(filePathREF) > 0)
		response.loadFileToMessageBody(filePathREF);
	else if (errno == EACCES)
		throw (403); //forbidden
	else
		throw (404);
	setHeader(response, filePathREF);
	return (response);
};

static void	callCGI(const std::string &filePathREF, const Request &requestREF, const ServerConfig::Location *locPTR) {
	CGIhandler	cgiHandler(requestREF, locPTR);

	if (http::checkMimeType(locPTR->locationUri) == MIME_CGI)
	{
		if (Autoindex::isPathReg(filePathREF) > 0 && Autoindex::isPathExec(filePathREF) > 0)
			cgiHandler.executeCGI(filePathREF);
		throw (404);
	}
}

static Response	handleGet(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	Response	response(200);

	callCGI(filePathREF, requestREF, locPTR);
	response = loadContent(filePathREF, requestREF, servConfREF, locPTR);
	return (response);
}

static Response handleHead(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR) {
    Response	response(200);

	response = handleGet(filePathREF, requestREF, servConfREF, locPTR);
	response.setMessageBody("");
	return (response);
}

static Response handlePut(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR) {
	FILE	*ofile;
	int		status;

	(void)servConfREF;
	(void)locPTR;
	status = 201; //created
	if (Autoindex::isPathExist(filePathREF) > 0)
		status = 204; //no content
	ofile = fopen(filePathREF.c_str(), "w");
	if (ofile != NULL)
	{
		http::filecpy(requestREF.getMessageBody(), ofile);
		close(fileno(ofile));
		fclose(ofile);
	}
	else
		status = 304; // not modified
	return (Response(status));
}

static Response handlePost(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR) {
	FILE	*ofile;

	(void)servConfREF;
	(void)locPTR;
	if (Autoindex::isPathExist(filePathREF) == 0)
	{
		ofile = fopen(filePathREF.c_str(), "w");
		if (ofile != NULL)
		{
			http::filecpy(requestREF.getMessageBody(), ofile);
			close(fileno(ofile));
			fclose(ofile);
			return (Response(201)); //created
		}
		throw (500);
	}
    return (Response(304)); // not modified
}

static Response handleDelete(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	(void) requestREF;
	(void) servConfREF;
	(void) locPTR;

	if (Autoindex::isPathExist(filePathREF) > 0 )
	{
		if (Autoindex::isPathReg(filePathREF) > 0 && remove(filePathREF.c_str()) == 0)
			return (Response(200)); //no content
		throw (403); //forbidden
	}
	throw (404);
}

static void	checkHost(const Request &requestREF, const ServerConfig &servConfREF)
{
	std::vector< std::string >	serverNames;
	std::string					hostHeader;

	hostHeader = ServerParser::splitByTwo(requestREF.getHeaderValue("Host"), ':').first;
	serverNames = servConfREF.getServerNames();
	if (hostHeader.empty() ||
		(!serverNames.empty() && std::find(serverNames.begin(), serverNames.end(), hostHeader) == serverNames.end()))
		throw (400);
}

static void	checkCMB(const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR)
{
	ssize_t	clientBodySize;

	// clientBodySize = requestREF.getMessageBodySize();
	clientBodySize = http::getFileSize(const_cast<FILE*>(requestREF.getMessageBody()));
	if (locPTR->getSizeCMB() > 0 && clientBodySize > locPTR->getSizeCMB())
		throw (413);
	else if (servConfREF.getSizeCMB() > 0 && clientBodySize > servConfREF.getSizeCMB())
		throw (413);
}

/**
 * @brief calls the right httpMethod and things to generate the proper response
 *
 * @param requestREF
 * @param serverConfigREF
 */
Response Response::buildResponse(const Request &requestREF, const ServerConfig &servConfREF)
{
	const ServerConfig::Location							*locPTR;
	std::string												filePath;
	std::pair< std::string, std::string >					uriNquery;

	locPTR = NULL;
	try {
		checkHost(requestREF, servConfREF);
		uriNquery = ServerParser::splitByTwo(requestREF.getUri(), QUERY_DELIM);
		locPTR = servConfREF.getLocation(uriNquery.first);
		if (locPTR != NULL && !(locPTR->limitExcept.acceptedMethods & requestREF.getHttpMethodEnum()))
			throw (405);
		if ((locPTR == NULL && servConfREF.getReturn().isInit) ||
				(locPTR != NULL && locPTR->getReturn().isInit))
			return (*this = RedirectResponse(servConfREF, locPTR));
		else if (locPTR == NULL)
			throw (404);
		checkCMB(requestREF, servConfREF, locPTR);
		filePath = getFilePath(requestREF, servConfREF, locPTR);
		switch (requestREF.getHttpMethodEnum())
		{
			case (HEAD):
			{
				*this = handleHead(filePath, requestREF, servConfREF, locPTR);
				break;
			}
			case (GET):
			{
				*this = handleGet(filePath, requestREF, servConfREF, locPTR);
				break;
			}
			case (PUT):
			{
				*this = handlePut(filePath, requestREF, servConfREF, locPTR);
				break ;
			}
			case (POST):
			{
				*this = handlePost(filePath, requestREF, servConfREF, locPTR);
				break ;
			}
			case (DELETE):
			{
				*this = handleDelete(filePath, requestREF, servConfREF, locPTR);
				break ;
			}
			default:
				throw (501);
		}
	}
	catch (int &errorCode) {
		return (this->Response::operator=(ErrorPageResponse(errorCode, servConfREF, locPTR)));
	}
	return (*this);
};

/**
 * @brief Retrieve the HTTP status string from the status code.
 *
 * @param statusCode The status code to retrieve the string from
 * @return std::string The HTTP status string
 */
std::string Response::getHttpStatusString(unsigned short statusCode) const
{
	switch (statusCode)
	{
		case (100):
			return ("Continue");
		case (101):
			return ("Switching Protocols");
		case (102):
			return ("Processing");
		case (103):
			return ("Early Hints");
		case (200):
			return ("OK");
		case (201):
			return ("Created");
		case (202):
			return ("Accepted");
		case (203):
			return ("Non-Authoritative Information");
		case (204):
			return ("No Content");
		case (205):
			return ("Reset Content");
		case (206):
			return ("Partial Content");
		case (207):
			return ("Multi-Status");
		case (208):
			return ("Already Reported");
		case (218):
			return ("This is fine");
		case (226):
			return ("IM Used");
		case (300):
			return ("Multiple Choices");
		case (301):
			return ("Moved Permanently");
		case (302):
			return ("Found");
		case (303):
			return ("See Other");
		case (304):
			return ("Not Modified");
		case (305):
			return ("Use Proxy");
		case (306):
			return ("Switch Proxy");
		case (307):
			return ("Temporary Redirect");
		case (308):
			return ("Permanent Redirect");
		case (400):
			return ("Bad Request");
		case (401):
			return ("Unauthorized");
		case (402):
			return ("Payment Required");
		case (403):
			return ("Forbidden");
		case (404):
			return ("Not Found");
		case (405):
			return ("Method Not Allowed");
		case (406):
			return ("Not Acceptable");
		case (407):
			return ("Proxy Authentication Required");
		case (408):
			return ("Request Timeout");
		case (409):
			return ("Conflict");
		case (410):
			return ("Gone");
		case (411):
			return ("Length Required");
		case (412):
			return ("Precondition Failed");
		case (413):
			return ("Payload Too Large");
		case (414):
			return ("URI Too Long");
		case (415):
			return ("Unsupported Media Type");
		case (416):
			return ("Range Not Satisfiable");
		case (417):
			return ("Expectation Failed");
		case (418):
			return ("I'm a teapot");
		case (419):
			return ("Page Expired");
		case (420):
			return ("Enhance Your Calm");
		case (421):
			return ("Misdirected Request");
		case (422):
			return ("Unprocessable Entity");
		case (423):
			return ("Locked");
		case (424):
			return ("Failed Dependency");
		case (425):
			return ("Too Early");
		case (426):
			return ("Upgrade Required");
		case (428):
			return ("Precondition Required");
		case (429):
			return ("Too Many Requests");
		case (431):
			return ("Request Header Fields Too Large");
		case (444):
			return ("No Response");
		case (450):
			return ("Blocked by Windows Parental Controls");
		case (451):
			return ("Unavailable For Legal Reasons");
		case (494):
			return ("Request Header Too Large");
		case (495):
			return ("SSL Certificate Error");
		case (496):
			return ("SSL Certificate Required");
		case (497):
			return ("HTTP Request Sent to HTTPS Port");
		case (498):
			return ("Invalid Token");
		case (499):
			return ("Token Required");
		case (500):
			return ("Internal Server Error");
		case (501):
			return ("Not Implemented");
		case (502):
			return ("Bad Gateway");
		case (503):
			return ("Service Unavailable");
		case (504):
			return ("Gateway Timeout");
		case (505):
			return ("HTTP Version Not Supported");
		case (506):
			return ("Variant Also Negotiates");
		case (507):
			return ("Insufficient Storage");
		case (508):
			return ("Loop Detected");
		case (509):
			return ("Bandwidth Limit Exceeded");
		case (510):
			return ("Not Extended");
		case (511):
			return ("Network Authentication Required");
		case (529):
			return ("Site is overloaded");
		case (530):
			return ("Site is frozen");
		case (598):
			return ("Network read timeout error");
		case (599):
			return ("Network connect timeout error");
		default:
			return ("");
	}
}
