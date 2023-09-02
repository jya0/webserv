#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include	<iostream>
# include	<string.h>
# include   <vector>
# include	<string>
# include	<sstream>
# include	<sys/socket.h>
# include	<sys/un.h>
# include	<arpa/inet.h>
# include	<fstream>
# include	<unistd.h>
# include	"cgi.hpp"
# include	"ServerSocket.hpp"
#include <cstddef>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
# define SERVERS_UP 2

class WebServer
{
protected:
	/* add stuff*/
	ServerSocket connection;
	std::vector<std::string> server_names;
	std::string defaultErrorPagePath;
	size_t max_body_size;
	std::vector<std::pair<std::string, std::string> > redirections;
	// std::byte methods;
	bool autoindex;
	std::string index;
	std::string root;
	bool CGI;
	std::string cgi-bin;
	Context	_serverConfig;

public:
    WebServer();
    WebServer(std::string ip, int port);
    ~WebServer();

	HTTPRequest &recieveRequest();
	void sendResponse(const HTTPResponse &response);
	HTTPResponse &handleRequest(const HTTPRequest &request);

	HTTPResponse handleCGI()

};

#endif