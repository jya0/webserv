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
# include <cstddef>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/event.h>
# include "../srcs/http/Http_namespace.hpp"
# define SERVERS_UP 2

using namespace http;

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

public:
    WebServer();
    WebServer(std::string ip, int port);
    ~WebServer();

	Request &recieveRequest();
	void sendResponse(const Response &response);
	Response &handleRequest(const Request &request);

	Response handleCGI()

};

#endif