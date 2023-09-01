/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/09/01 20:11:21 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServer.hpp"
#include "../inc/HTTPServerParser.hpp"

void StartServers(std::vector<WebServer> &servers)
{
	struct kevent change_event[4], event[4];
	// Prepare the kqueue.
	int kq = kqueue();
	int new_events;
	// Create event 'filter', these are the events we want to monitor.
	// Here we want to monitor: socket_listen_fd, for the events: EVFILT_READ
	// (when there is data to be read on the socket), and perform the following
	// actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue
	// and enable it).
	EV_SET(change_event, servers[0].getPassiveSocket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

	// Register kevent with the kqueue.
	if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		exit(1);
	}
	while (2)
	{
		// Check for new events, but do not register new events with
		// the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
		// Only handle 1 new event per iteration in the loop; 5th
		// argument is 1.
		new_events = kevent(kq, NULL, 0, event, 1, NULL);
		if (new_events == -1)
		{
			perror("kevent");
			exit(1);
		}

		for (int i = 0; i < new_events; i++)
		{
			int event_fd = event[i].ident;

			// When the client disconnects an EOF is sent. By closing the file
			// descriptor the event is automatically removed from the kqueue.
			if (event[i].flags & EV_EOF)
			{
				printf("Client has disconnected");
				close(event_fd);
			}
			// If the new event's file descriptor is the same as the listening
			// socket's file descriptor, we are sure that a new client wants
			// to connect to our socket.
			else if (event_fd == servers[0].getPassiveSocket())
			{
				// Incoming socket connection on the listening socket.
				// Create a new socket for the actual connection to client.
				servers[0].AcceptConnection();
				if (servers[0].getPeerSocket() == -1)
				{
					perror("Accept socket error");
				}

				// Put this new socket connection also as a 'filter' event
				// to watch in kqueue, so we can now watch for events on this
				// new socket.
				EV_SET(change_event, servers[0].getPeerSocket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0)
				{
					perror("kevent error");
				}
			}

			else if (event[i].filter & EVFILT_READ)
			{
				// Read bytes from socket
				char buf[1024];
				// size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
				servers[0].RecieveData();
				// printf("read %zu bytes\n", bytes_read);
			}
			else if (event[i].filter & EVFILT_WRITE)
			{
				servers[0].SendData();
				// printf("read %zu bytes\n", bytes_read);
			}
		}


	}
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	WebServer s1 = WebServer("127.0.0.1", 8080);
	std::vector<WebServer> servers;


	servers.push_back(s1);
	if (argc != 2)
	{
		std::cout<<"Invalid Paramaters!\n";
		std::cout << "./web_serve [config_file_name]\n";
		return (0);
	}
	try
	{
		servers = HTTPServerParser::parseConfigFile(argv[1]);
		StartServers(servers);
	}
	catch (HTTPServerParser::ParseError &e)
	{
		// handle parse error
	}
	catch(WebServer::SocketIOError)
	{
		// handle socket IO error
	}
	catch (...)
	{

	}


	// server.startListening();
	return (0);
}