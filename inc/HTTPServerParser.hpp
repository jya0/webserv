#ifndef HTTP_SERVER_PARSER_HPP
#define HTTP_SERVER_PARSER_HPP

#include "WebServer.hpp"

class HTTPServerParser {
private:
    HTTPServerParser();

public:
    static std::vector<WebServer> &parseConfigFile(std::string filename);
    ~HTTPServerParser();

    class ParseError : public std::exception {
    public:
        virtual const char *what() const throw();
    };
};


#endif