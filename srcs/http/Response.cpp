/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 18:30:42 by jyao              #+#    #+#             */
/*   Updated: 2023/10/29 06:23:23 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"Http_namespace.hpp"

using namespace http;

/**
 * @brief Construct a new Response object (default constructor)
 */
Response::Response(void) : AMessage(){
    _ready = false;
	return ;
}

/**
 * @brief Construct a new Response object from a status code.
 *
 * @param status The status code of the response
 */
Response::Response(int status) {
    this->_httpVersion = "HTTP/1.1";
    this->_httpStatusCode = status;
    this->_startLine = this->_httpVersion + " " + std::to_string(status) + " " + this->getHttpStatusString(status);
    _ready = true;
	return ;
}
/**
 * @brief Construct a new Response object (copy constructor)
 *
 * @param responseREF Response object to copy
 */
Response::Response(const Response &responseREF) {
    *this = responseREF;
    return ;
}

/**
 * @brief Destroy the Response object
 */
Response::~Response(void) {
    return ;
}

/**
 * @brief The Response copy assignment operator.
 *
 * @param responseREF Response object to copy
 * @return Response& Reference to the new Response object
 */
Response	&Response::operator=(const Response &responseREF) {
    if (this != &responseREF) {
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
Response::Response(std::string httpRaw): AMessage(httpRaw) {

	this->_httpVersion = this->_startLine.substr(0, this->_startLine.find(' '));
    this->_httpStatusCode = std::stoi(this->_startLine.substr(this->_startLine.find(' ') + 1,
                this->_startLine.find(' ', this->_startLine.find(' ') + 1) -
                this->_startLine.find(' ') - 1));
	_ready = true;
}

/**
 * @brief Return the HTTP version of the response.
 *
 * @return std::string The HTTP version of the response
 */
std::string	Response::getHttpVersion(void) const {
    return (this->_httpVersion);
}

/**
 * @brief Return the HTTP status code of the response.
 *
 * @return unsigned short The HTTP status code of the response
 */
unsigned short	Response::getHttpStatusCode(void) const {
    return (this->_httpStatusCode);
}


bool Response::responseReady() const {
	return (_ready);
}

void Response::setResponseStatus(bool status) {
	_ready = status;
}


/**
 * @brief Retrieve the HTTP status string from the status code.
 *
 * @param statusCode The status code to retrieve the string from
 * @return std::string The HTTP status string
 */
std::string Response::getHttpStatusString(unsigned short statusCode) const {
    switch (statusCode) {
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

/**
 * @brief Return the HTTP status string of the response.
 *
 * @return std::string The HTTP status string of the response
 */
bool    Response::validate(void) const {
    return (true);
}