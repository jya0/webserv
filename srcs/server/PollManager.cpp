/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 05:03:53 by rriyas            #+#    #+#             */
/*   Updated: 2023/11/30 11:48:12 by rriyas           ###   ########.fr       */
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
	sockets[nfds].fd = fd;
	sockets[nfds].events = events;
	sockets[nfds].revents = 0;
	nfds++;
}

void PollManager::removeFd(int fd) {
	//1. find index of fd to remove
	int i = 0;
	for (i = 0; i < nfds; i++)
		if (sockets[i].fd == fd)
			break ;
	//2. skip element and copy bacwards until the end
	if (i == nfds)
		return ;
	i++;
	while (i < nfds) {
		sockets[i] = sockets[i+1];
		i++;
	}
	sockets[i].fd = 0;
	sockets[i].events = 0;
	sockets[i].revents = 0;
	nfds--;
}
