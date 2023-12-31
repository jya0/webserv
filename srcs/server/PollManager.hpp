#ifndef POLL_MANAGER_HPP
# define POLL_MANAGER_HPP

#include <vector>
#include <iostream>
#include <sys/poll.h>
#include <cstring>

#define MAX_SOCKETS 10000


class PollManager {
private:
	size_t					nfds;
	struct pollfd			_sockets[MAX_SOCKETS];

public:
	PollManager(size_t);
	PollManager(const PollManager &rhs);
	PollManager &operator=(const PollManager &rhs);
	~PollManager();

	pollfd	&operator[](int);


	void	addFd(size_t fd, short events);
	void	removeFd(size_t fd);
	int		callPoll();
	int		getNfds();
};

#endif