#include "../../inc/HTTPServerParser.hpp"


std::vector<WebServer> &HTTPServerParser::parseConfigFile(std::string filename)
{
    //code to parse webserv config file goes here
	std::vector<WebServer> *servers = new std::vector<WebServer>();
	return (*servers);
}

const char *HTTPServerParser::ParseError::what() const throw()
{
    return ("Error while parsing file!\n");
}
HTTPServerParser::HTTPServerParser()
{

}

HTTPServerParser::~HTTPServerParser()
{

}