#ifndef POLL_MANAGER_HPP
# define POLL_MANAGER_HPP

#include <vector>
#include <iostream>
#include <sys/poll.h>

class PollManager
{
private:
	size_t				nfds;
	// std::vector<pollfd> sockets;
	struct pollfd sockets[200];

public:
	PollManager(size_t);
	~PollManager();

	pollfd &operator[](int);
	void operator++(int);
	void operator--(int);


	void addFd(int fd, short events);
	void removeFd(int fd);
	int callPoll();
	int getNfds();
};

#endif