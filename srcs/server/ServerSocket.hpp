#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

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
# include	<fcntl.h>
# include	<exception>
# include "AMessage.hpp"

using namespace http;

# define	BUFFER_SIZE 500
# define	SERVER_QUEUE_SIZE 42


class ServerSocket {
protected:
	std::string			ip_address;
	int					port;
	int					passive_socket;
	struct sockaddr_in	socket_address;
	unsigned int		socket_address_len;

public:
	ServerSocket();
	ServerSocket(std::string ip_addr, int port);
	ServerSocket(const ServerSocket &rhs);
	ServerSocket &operator=(const ServerSocket &rhs);
	~ServerSocket();

	ssize_t				sendData(int &peer_socket, std::string message);
	ssize_t				recieveData(int &peer_socket, char *buffer);
	void				startConnection();
	void				startListening();
	int					acceptConnection();
	void				closeConnection();
	const int			&getPassiveSocket();
	class SocketIOError: public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

#endif