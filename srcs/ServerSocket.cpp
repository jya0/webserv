#include "../inc/ServerSocket.hpp"

static void log(std::string mesg)
{
	std::cout << mesg << std::endl;
}

ServerSocket::ServerSocket()
{
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	peer_socket = -1;
	startConnection();
}

ServerSocket::ServerSocket(std::string ip_addr, int port) : ip_address(ip_addr), port(port),
													  passive_socket(), peer_socket(-1), socket_address(),
													  socket_address_len(sizeof(socket_address))
{
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	startConnection();
}

ServerSocket::~ServerSocket()
{
	closeConnection();
}

std::string ServerSocket::generateDefaultResponse()
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
	   << htmlFile;
	return ss.str();
}

std::string ServerSocket::recieveData()
{
	long bytesRecieved;
    char buffer[BUFFER_SIZE] = {0};

	//replace read with receieve
	bytesRecieved = recv(peer_socket, buffer, BUFFER_SIZE, 0);
	if (bytesRecieved < 0)
	{
		log("Failed to read bytes from client socket connection\n");
		exit(0);
	}

	std::ostringstream ss;
	ss << "------ Received Request from client ------\n\n";
	std::cout<<std::string(buffer)<<"\n";
	log(ss.str());

	// RecieveData();
	// close(peer_socket);
	return ("");
}

void ServerSocket::sendData(std::string message)
{
    long bytesSent;
    //replace write with SEND function
	const char *s = message.c_str();
	bytesSent = write(peer_socket, s, message.size());
	if (bytesSent == message.size())
		log("------ Server Response sent to client ------\n\n");
	else
		log("Error sending response to client");
}

void ServerSocket::startConnection()
{
	passive_socket = socket(AF_INET, SOCK_STREAM, 0);
	const int enable = 1;
	// setsockopt(passive_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	// setsockopt(passive_socket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));

	if (passive_socket < 0)
	{
		log("Socket creation failed\n");
		return ;
	}
	if (bind(passive_socket, (sockaddr *)&socket_address, socket_address_len) < 0)
	{
		log("Cannot connect socket to address");
		return ;
	}
	fcntl(passive_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	// std::cout<<"Socket connection created and started succesfully!\n";
}

void ServerSocket::startListening()
{
    if (listen(passive_socket, 20) < 0)
	{
		log("Socket listen failed\n");
	}
	std::ostringstream ss;
	ss << "\n*** Listening on ADDRESS: "
	   << inet_ntoa(socket_address.sin_addr)
	   << " PORT: " << ntohs(socket_address.sin_port)
	   << " ***\n\n";
	log(ss.str());
}

void ServerSocket::acceptConnection()
{
    peer_socket = accept(passive_socket, (sockaddr *)&socket_address,
						&socket_address_len);
	if (peer_socket < 0)
	{
		std::ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: "
		   << inet_ntoa(socket_address.sin_addr) << "; PORT: "
		   << ntohs(socket_address.sin_port);
		log(ss.str());
	}
	fcntl(peer_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	// std::cout<<"Connection accepted!\n";
}

void ServerSocket::closeConnection()
{
	close(peer_socket);
	peer_socket = -1;
	// close(passive_socket);
}


const int &ServerSocket::getPeerSocket()
{
	return (peer_socket);
}

const int &ServerSocket::getPassiveSocket()
{
	return (passive_socket);
}


const char *ServerSocket::SocketIOError::what() const throw()
{
    return ("Socket IO error!\n");
}