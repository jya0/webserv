#include "../../inc/HTTPServerParser.hpp"


std::vector<TCPSocket> &HTTPServerParser::parseConfigFile(std::string filename)
{
    //code to parse webserv config file goes here
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