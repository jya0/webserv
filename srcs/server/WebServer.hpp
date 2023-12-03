#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include	<iostream>
# include	<string.h>
# include 	<map>
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
# include	<cstddef>
# include	<netinet/in.h>
# include	<sys/types.h>
# include	<sys/event.h>
# include	<sys/ioctl.h>
# include	<poll.h>
# include	<stdio.h>
# include	"ServerConfig.hpp"
# include	"Http_namespace.hpp"
# include	"ServerSocket.hpp"
# include	"PollManager.hpp"

# define	SERVERS_UP 2

using namespace http;

class	DirectiveBlock;
class WebServer {
protected:
	ServerSocket connection;
	std::vector<int> clients;

	ServerConfig	_config;

public:
	std::map<int, Response *> responses;
	std::map<int, Request *> requests;

	WebServer();
    WebServer(std::string ip, int port);
    ~WebServer();
	WebServer(const ServerConfig &configREF);
	ServerSocket &getConnection();
	Request &recieveRequest();
	void sendResponse(int client, const Response &response);

	void sendData(int client, std::string message);
	int recieveData(int &client);
	Request receiveRequest(int client, std::string );
	// Response handleRequest(const Request &requestREF) const ;
	// Response handleGet(const Request &requestREF) const ;
	// Response handlePost(const Request &requestREF) const ;
	// Response handlePut(const Request &requestREF) const ;
	// Response handleDelete(const Request &requestREF) const ;
	// Response handleHead(const Request &requestREF) const ;
	// Response handleCGI()const ;

	void startServer();
	void startConnection();
	void startListening();
	int acceptConnection();
	void closeServerConnection();
	void closeClientConnection(int client);
	bool connectedClient(int client) const ;
	bool responseReady(int client);

	void buildResponse(int client);
	bool requestReady(int client);
	void closeCGI(CGIhandler &cgiREF);
};

#endif