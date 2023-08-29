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

#define BUFFER_SIZE 20000
class TCPSocket
{
private:
	std::string ip_address;
	int port;
	int passive_socket;
	int peer_socket;
	struct sockaddr_in socket_address;
	unsigned int socket_address_len;
	std::string response;

public:
	TCPSocket();
	TCPSocket(std::string ip_addr, int port);
	~TCPSocket();

	std::string GenerateDefaultResponse();
	
	void SendData();
	void RecieveData();


	int StartConnection();
	void StartListening();
	void AcceptConnection();
	void CloseConnection();

	class SocketIOError : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

#endif