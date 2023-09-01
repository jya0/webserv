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
	StartConnection();
	StartListening();
}

ServerSocket::ServerSocket(std::string ip_addr, int port) : ip_address(ip_addr), port(port),
													  passive_socket(), peer_socket(), socket_address(),
													  socket_address_len(sizeof(socket_address)), response(ServerSocket::GenerateDefaultResponse())
{
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	StartConnection();
}

ServerSocket::~ServerSocket()
{
	CloseConnection();
}

std::string ServerSocket::GenerateDefaultResponse()
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
	   << htmlFile;
	return ss.str();
}

std::string ServerSocket::RecieveData()
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
	log(ss.str());

	// RecieveData();
	close(peer_socket);
	return ("");
}

void ServerSocket::SendData(std::string message)
{
    long bytesSent;
    //replace write with SEND function
	bytesSent = write(peer_socket, message.c_str(), message.size());

	if (bytesSent == message.size())
		log("------ Server Response sent to client ------\n\n");
	else
		log("Error sending response to client");
}

void ServerSocket::StartConnection()
{
	passive_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (passive_socket < 0)
	{
		log("Socket creation failed\n");
		return (1);
	}
	if (bind(passive_socket, (sockaddr *)&socket_address, socket_address_len) < 0)
	{
		log("Cannot connect socket to address");
		return (1);
	}

}

void ServerSocket::StartListening()
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

void ServerSocket::AcceptConnection()
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
	std::cout<<"Connection accepted!\n";
}

void ServerSocket::CloseConnection()
{
	close(passive_socket);
	close(peer_socket);
	exit(0);
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