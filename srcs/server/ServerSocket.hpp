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
# include <fcntl.h>
#define BUFFER_SIZE 1

# define	BUFFER_SIZE 10

class ServerSocket {
protected:
	std::string ip_address;
	int port;
	int passive_socket;
	struct sockaddr_in socket_address;
	unsigned int socket_address_len;

public:
	ServerSocket();
	ServerSocket(std::string ip_addr, int port);
	~ServerSocket();

	static std::string generateDefaultResponse();

	void sendData(int peer_socket, std::string message);
	std::string recieveData(int &peer_socket);


	void startConnection();
	void startListening();
	int acceptConnection();
	void closeConnection();


	const int &getPassiveSocket();


	class SocketIOError : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

#endif