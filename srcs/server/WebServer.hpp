#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fstream>
#include <istream>
#include <unistd.h>
#include <cstddef>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstddef>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <stdio.h>
#include "ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "PollManager.hpp"

#include "Response.hpp"
#include "CGIhandler.hpp"
#include "Request.hpp"

#define SERVERS_UP 2

using namespace http;

class DirectiveBlock;
class WebServer
{
	protected:
		ServerSocket				_connection;
		std::vector< int >			_clients;
		ServerConfig				_config;
	public:
		std::map<int, Response >	responses;
		std::map<int, Request >		requests;

		WebServer();
		WebServer(std::string ip, int port);
		WebServer(const WebServer &rhs);
		WebServer &operator=(const WebServer &rhs);
		~WebServer();
		WebServer(const ServerConfig &configREF);

		ServerSocket	&getConnection();
		ssize_t			sendResponse(int client, Response &response);
		ssize_t			sendData(int client, std::string message);
		ssize_t			recieveData(int &client);
		void			startConnection();
		void			startListening();
		int				acceptConnection();
		void			closeServerConnection();
		void			closeClientConnection(int client);
		bool			connectedClient(int client) const;
		bool			responseReady(int client);
		void			buildResponse(int client);
		bool			requestReady(int client);
		void			closeCGI(const CGIhandler &cgiREF, const int &statusREF);
};

#endif