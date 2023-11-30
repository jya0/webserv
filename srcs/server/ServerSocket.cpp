#include "../../inc/ServerSocket.hpp"

static void log(std::string mesg) {
	std::cout << mesg << std::endl;
}

ServerSocket::ServerSocket() {
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	startConnection();
}

ServerSocket::ServerSocket(std::string ip_addr, int port) : ip_address(ip_addr), port(port),
													  passive_socket(), socket_address(),
													  socket_address_len(sizeof(socket_address)) {
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);
	socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
	try {
		startConnection();
	} catch(...) {
		std::cerr<<"Faled to establish connection\n";
		exit(0);
	}
}

ServerSocket::~ServerSocket() {
	closeConnection();
}

std::string ServerSocket::generateDefaultResponse() {
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " << htmlFile.size() << "\r\n\r\n"
	   << htmlFile;
	return ss.str();
}

std::string ServerSocket::recieveData(int *peer_socket) {
	long bytesRecieved;
    char *buffer = new char[BUFFER_SIZE + 1];

	memset(buffer, 0, BUFFER_SIZE + 1);

	bytesRecieved = recv(*peer_socket, buffer, BUFFER_SIZE, 0);
	if (bytesRecieved == 0)
	{
		*peer_socket = -1;
		delete []buffer;
		return (std::string(""));
	}
	if (bytesRecieved < 0)
	{
		log("Failed to read bytes from client socket connection\n");
		delete []buffer;
		exit(0);
	}

	std::ostringstream ss;
	ss << "------ Received Request from client ------\n\n";
	std::cout<<std::string(buffer)<<"\n";
	log(ss.str());
	ss.clear(); // clear any bits set
	ss.str(std::string());
	std::string ret = std::string(buffer);
	delete []buffer;
	return (ret);
}

void ServerSocket::sendData(int peer_socket, std::string message) {
    long bytesSent;
	const char *s = message.c_str();
	std::cerr<<message;
	bytesSent = send(peer_socket, s, message.size(), 0);
	if (bytesSent == message.size())
		log("------ Server Response sent to client ------\n\n");
	else
		log("Error sending response to client");
}

void ServerSocket::startConnection() {
	passive_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (passive_socket < 0)
	{
		log("Socket creation failed\n");
		return ;
	}
	if (bind(passive_socket, (sockaddr *)&socket_address, socket_address_len) < 0)
	{
		log("Cannot connect socket to address");
		throw SocketIOError();
		return ;
	}
	fcntl(passive_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
}

void ServerSocket::startListening() {
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

int ServerSocket::acceptConnection() {
    int peer_socket = accept(passive_socket, (sockaddr *)&socket_address,
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
	std::cout<<"Connection accepted!\n";
	return (peer_socket);
}

void ServerSocket::closeConnection() {
	close(passive_socket);
}

const int &ServerSocket::getPassiveSocket() {
	return (passive_socket);
}


const char *ServerSocket::SocketIOError::what() const throw() {
    return ("Socket IO error!\n");
}