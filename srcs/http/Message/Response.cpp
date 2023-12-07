/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:30:42 by jyao              #+#    #+#             */
/*   Updated: 2023/12/07 17:04:32 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<fstream>
#include	<sstream>
#include	<cstdlib>
#include	"Http_namespace.hpp"

using namespace http;

/**
 * @brief Construct a new Response object (default constructor)
 */
Response::Response(void) : AMessage()
{
	_ready = false;
	return;
}

/**
 * @brief Construct a new Response object from a status code.
 *
 * @param status The status code of the response
 */
Response::Response(int status)
{
	std::stringstream	ss;

	this->_httpVersion = "HTTP/1.1";
	this->_httpStatusCode = status;
	ss << this->_httpVersion << " " << status << " " << this->getHttpStatusString(status) << CR_LF;
	this->_startLine = ss.str();
	this->setMessageBody(this->getHttpStatusString(status));
	this->addHeader(Header("Content-Type", "text/html"));
	this->addHeader(Header("Content-Length", http::toString(this->getMessageBody().length())));
	_ready = true;
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
Response::Response(const Response &responseREF) : AMessage(responseREF)
{
	*this = responseREF;
	return;
}

/**
 * @brief Destroy the Response object
 */
Response::~Response(void)
{
	return;
}

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
	}
	return (*this);
}

/**
 * @brief Construct a new Response object from a string.
 *
 * @param httpRaw The string to parse
 */
Response::Response(std::string httpRaw) : AMessage(httpRaw)
{

	_httpVersion		=	this->_startLine.substr(0, this->_startLine.find(' '));
	_httpStatusCode		=	strtol(this->_startLine.substr(this->_startLine.find(' ') + 1,
								this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
								this->_startLine.find(' ') - 1).c_str(), NULL, 10);
	_ready = true;
}

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

static std::string getFilePath(const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF)
{
	std::string filePath;

	filePath = locREF.getRoot();
	filePath = filePath.empty() ? servConfREF.getRoot() : filePath;
	filePath += requestREF.getUri();
	return (filePath);
}

static void header_GetHead(Response &response, const std::string &fileStr)
{
	Header length("Content-Length", http::toString(fileStr.length()));
	Header type("Content-Type", "text/html");
	Header server("Server", "webserv-kry");
	// Header date("Date", getDate());

	response.addHeader(length);
	response.addHeader(type);
}

Response readContent(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF)
{
	Response response(200);
	std::string result;
	std::ifstream infile;

	if (Autoindex::isPathFolder(filePathREF) > 0)
	{
		if (locREF.getAutoIndex() == true)
			result = Autoindex::genPage(filePathREF.c_str(), requestREF, servConfREF);
		else
			return (Response(403));
	}
	else if (Autoindex::isPathReg(filePathREF) > 0)
	{
		infile.open(filePathREF.c_str(), std::ios::in);
		if (infile.good())
		{
			result = std::string((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
			infile.close();
		}
	}
	else
	{
		if (errno == EACCES)
			return (Response(403));
	}
	header_GetHead(response, result);
	response.setMessageBody(result);
	return (response);
};

static void	callCGI(const std::string &filePathREF, const Request &requestREF, const ServerConfig::Location &locREF) {
	CGIhandler	cgiHandler(requestREF, locREF);

	for (http::t_mime_map::const_iterator itc = http::mimeTypes.begin(); itc != http::mimeTypes.end(); ++itc)
	{
		for (std::vector < std::string >::const_iterator itc2 = itc->first.begin(); itc2 != itc->first.end(); ++itc2)
		{
			std::cout << *itc2 << " ";
		}
		std::cout << ":" << itc->second << std::endl;
	}
	if (!http::checkMimeType(requestREF.getUri()).empty() && Autoindex::isPathReg(filePathREF) > 0 && Autoindex::isPathExec(filePathREF) > 0)
	{
		cgiHandler.executeCGI(filePathREF);
	}
	std::cerr << "I DIDN'T RUN CGI" << std::endl;
}

static Response handleHead(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF) {
    Response	response(200);
	// CGIhandler	cgiHandler(requestREF, locREF);

	//@todo: The HTTP GET method requests a representation of the specified resource. Requests using GET should only be used to
	// request data (they shouldn't include data).

	response = readContent(filePathREF, requestREF, servConfREF, locREF);
	response.setMessageBody("");
	return (response);
}

static Response handleGet(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF)
{
	Response response(200);
	//@todo: The HTTP GET method requests a representation of the specified resource. Requests using GET should only be used to
	// request data (they shouldn't include data).

	// Flow:
	// code for CGI checking function
	// if (locREF.locationUri )
	callCGI(filePathREF, requestREF, locREF);
	response = readContent(filePathREF, requestREF, servConfREF, locREF);
	return (response);
}

// static Response handlePut(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF) {
//     Response response(200);
//     // @todo
// 	/*
// 		Flow:

// 	*/
//     return (response);
// }

static Response handlePost(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF) {
	Response response(200);

	return (response);
}

static Response handleDelete(const std::string &filePathREF, const Request &requestREF, const ServerConfig &servConfREF, const ServerConfig::Location &locREF)
{
	(void) requestREF;
	(void) servConfREF;
	(void) locREF;

	if (Autoindex::isPathFolder(filePathREF) > 0 || Autoindex::isPathReg(filePathREF) > 0) {
		if (remove(filePathREF.c_str()) == 0)
			return (Response(204));
		else
			return (Response(403));
	}
	else
		return (Response(404));
	return (Response(status, "<html><body><h1>File deleted.</h1></body></html>"));
}

/**
 * @brief calls the right httpMethod and things to generate the proper response
 *
 * @param requestREF
 * @param serverConfigREF
 */
Response Response::buildResponse(const Request &requestREF, const ServerConfig &servConfREF)
{
	std::vector<ServerConfig::Location>::const_iterator locItc;
	std::string filePath;

	// if (requestREF.getMessageBody().size() > servConfREF.getSizeCMB())
	// 	return (*this = Response(413));
	locItc = servConfREF.getLocation(requestREF.getUri());
	// if (!(locItc->limitExcept.acceptedMethods & requestREF.getHttpMethodEnum()))
	// 	return (*this = Response(405));
	// if (locItc == servConfREF.getLocations().end() && !locItc->getReturn().second.empty())
	// 	return (*this = Response(locItc->getReturn().first, "Location: " + locItc->getReturn().second));
	// else
	// 	return (*this = Response(404));
	filePath = getFilePath(requestREF, servConfREF, *locItc);
	switch (requestREF.getHttpMethodEnum())
	{
	case HEAD:
		*this = handleHead(filePath, requestREF, servConfREF, *locItc);
		break;
	case GET:
		*this = handleGet(filePath, requestREF, servConfREF, *locItc);
		break;
	// case PUT:
	// 	*this = handlePut();
	// case POST:
	// 	*this = handlePost();
	case DELETE:
		*this = handleDelete(filePath, requestREF, servConfREF, *locItc);
		break ;
	default:
		*this = Response(501);
		break;
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
	case 100:
		return ("Continue");
	case 101:
		return ("Switching Protocols");
	case 102:
		return ("Processing");
	case 103:
		return ("Early Hints");
	case 200:
		return ("OK");
	case 201:
		return ("Created");
	case 202:
		return ("Accepted");
	case 203:
		return ("Non-Authoritative Information");
	case 204:
		return ("No Content");
	case 205:
		return ("Reset Content");
	case 206:
		return ("Partial Content");
	case 207:
		return ("Multi-Status");
	case 208:
		return ("Already Reported");
	case 218:
		return ("This is fine");
	case 226:
		return ("IM Used");
	case 300:
		return ("Multiple Choices");
	case 301:
		return ("Moved Permanently");
	case 302:
		return ("Found");
	case 303:
		return ("See Other");
	case 304:
		return ("Not Modified");
	case 305:
		return ("Use Proxy");
	case 306:
		return ("Switch Proxy");
	case 307:
		return ("Temporary Redirect");
	case 308:
		return ("Permanent Redirect");
	case 400:
		return ("Bad Request");
	case 401:
		return ("Unauthorized");
	case 402:
		return ("Payment Required");
	case 403:
		return ("Forbidden");
	case 404:
		return ("Not Found");
	case 405:
		return ("Method Not Allowed");
	case 406:
		return ("Not Acceptable");
	case 407:
		return ("Proxy Authentication Required");
	case 408:
		return ("Request Timeout");
	case 409:
		return ("Conflict");
	case 410:
		return ("Gone");
	case 411:
		return ("Length Required");
	case 412:
		return ("Precondition Failed");
	case 413:
		return ("Payload Too Large");
	case 414:
		return ("URI Too Long");
	case 415:
		return ("Unsupported Media Type");
	case 416:
		return ("Range Not Satisfiable");
	case 417:
		return ("Expectation Failed");
	case 418:
		return ("I'm a teapot");
	case 419:
		return ("Page Expired");
	case 420:
		return ("Enhance Your Calm");
	case 421:
		return ("Misdirected Request");
	case 422:
		return ("Unprocessable Entity");
	case 423:
		return ("Locked");
	case 424:
		return ("Failed Dependency");
	case 425:
		return ("Too Early");
	case 426:
		return ("Upgrade Required");
	case 428:
		return ("Precondition Required");
	case 429:
		return ("Too Many Requests");
	case 431:
		return ("Request Header Fields Too Large");
	case 444:
		return ("No Response");
	case 450:
		return ("Blocked by Windows Parental Controls");
	case 451:
		return ("Unavailable For Legal Reasons");
	case 494:
		return ("Request Header Too Large");
	case 495:
		return ("SSL Certificate Error");
	case 496:
		return ("SSL Certificate Required");
	case 497:
		return ("HTTP Request Sent to HTTPS Port");
	case 498:
		return ("Invalid Token");
	case 499:
		return ("Token Required");
	case 500:
		return ("Internal Server Error");
	case 501:
		return ("Not Implemented");
	case 502:
		return ("Bad Gateway");
	case 503:
		return ("Service Unavailable");
	case 504:
		return ("Gateway Timeout");
	case 505:
		return ("HTTP Version Not Supported");
	case 506:
		return ("Variant Also Negotiates");
	case 507:
		return ("Insufficient Storage");
	case 508:
		return ("Loop Detected");
	case 509:
		return ("Bandwidth Limit Exceeded");
	case 510:
		return ("Not Extended");
	case 511:
		return ("Network Authentication Required");
	case 529:
		return ("Site is overloaded");
	case 530:
		return ("Site is frozen");
	case 598:
		return ("Network read timeout error");
	case 599:
		return ("Network connect timeout error");
	default:
		return ("");
	}
}
