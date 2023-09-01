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
# include	"TCPSocket.hpp"
# include   "HTTPServerParser.hpp"

class WebServer
{
private:
    std::vector<TCPSocket> sockets;

    void StartServer();
public:
    WebServer();
    WebServer(std::string config);
    WebServer(std::string ip, int port);

    ~WebServer();
};

WebServer::WebServer() {
}

WebServer::WebServer(std::string ip, int port) {
    StartServer();
}


WebServer::WebServer(std::string config) {
    try {
        sockets = HTTPServerParser::parseConfigFile(config);
        
        //start listening on all sockets
        for (std::vector<TCPSocket>::iterator it = sockets.begin(); it != sockets.end(); it++)
            it->StartListening();
        StartServer();   
    }
    catch(HTTPServerParser::ParseError &e) {
        //handle parse error
    }
    catch (TCPSocket::SocketIOError &e){
        //socker error from startServer
    }
    catch(...) {

    }
}

void WebServer::StartServer() {

    //main loop for kqueue goes here to accept connections
    while (2)
    {

    }
}

WebServer::~WebServer() {
}



#endif