/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 05:03:53 by rriyas            #+#    #+#             */
/*   Updated: 2023/10/29 06:29:35 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/PollManager.hpp"


PollManager::PollManager(size_t n) : nfds(n){
}

PollManager::~PollManager() {
}

int PollManager::callPoll() {
	return (poll(&sockets[0], nfds, -1));
}

pollfd &PollManager::operator[](int index) {
	return (sockets[index]);
}

int PollManager::getNfds() {
	return (nfds);
}


void PollManager::addFd(int fd, short events) {
	pollfd socket;

	socket.fd = fd;
	socket.events = events;
	socket.revents = 0;
	sockets.push_back(socket);
	nfds++;
}

void PollManager::removeFd(int fd) {
	for (std::vector<pollfd>::iterator itr = sockets.begin(); itr != sockets.end(); itr++) {
		if (itr->fd == fd) {
			sockets.erase(itr);
			nfds--;
			return ;
		}
	}
}
