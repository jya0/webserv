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
# include	"TCPServer.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>

# define SERVERS_UP 2

class WebServer : public TCPServer
{
private:
	/* add stuff*/
public:
    WebServer();
    WebServer(std::string ip, int port);
    ~WebServer();

	void InitiateServer();
};


#endif