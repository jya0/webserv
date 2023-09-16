#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include	<iostream>
# include	<string.h>
#include <map>
# include   <vector>
# include	<utility>
# include	<string>
# include	<sstream>
# include	<sys/socket.h>
# include	<sys/un.h>
# include	<arpa/inet.h>
# include	<fstream>
# include	<istream>
# include	<unistd.h>
# include	<cstddef>
# include	<netinet/in.h>
# include	<sys/socket.h>
# include	"../srcs/http/Http_namespace.hpp"
// # include	"../srcs/ServerConfig/ServerConfig_namespace.hpp"
# include	"cgi.hpp"
# include	"ServerSocket.hpp"
# include <cstddef>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/ioctl.h>
# include "../srcs/http/Http_namespace.hpp"
# define SERVERS_UP 2

using namespace http;

class WebServer {
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
	std::string cgi_bin;
	// DirectiveBlock	_serverConfig;

public:
    WebServer();
	// WebServer(const DirectiveBlock &serverBlockREF);
    WebServer(std::string ip, int port);
    ~WebServer();

	ServerSocket &getConnection();
	Request &recieveRequest();
	void sendResponse(const Response &response);

	void sendData(std::string message);
	std::string recieveData();
	Request receiveRequest(std::string );
	Response handleRequest(const Request &request) const ;
	Response handleGet(const Request &request) const ;
	Response handlePost(const Request &request) const ;
	Response handlePut(const Request &request) const ;
	Response handleDelete(const Request &request) const ;
	Response handleHead(const Request &request) const ;
	Response handleCGI()const ;

	void startConnection();
	void startListening();
	void acceptConnection();
	void closeConnection();



};

#endif