#ifndef SERVER_HPP
# define SERVER_HPP
# define BUFFER_SIZE 100000
# include "webserv.hpp"

	class TCPServer
	{
	private:
		std::string m_ip_address;
		int m_port;
		int m_socket;
		int m_new_socket;
		long m_incomingMessage;
		struct sockaddr_in m_socketAddress;
		unsigned int m_socketAddress_len;
		std::string m_serverMessage;
	public:
		TCPServer();
		TCPServer(std::string ip_addr, int port);
		~TCPServer();

		int startServer();
		void closeServer();
		void startListening();
		void acceptConnection(int &socket);

		std::string buildResponse();
		void sendResponse();
	};

#endif