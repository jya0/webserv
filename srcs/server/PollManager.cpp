/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 05:03:53 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/03 13:54:02 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"


PollManager::PollManager(size_t n) : nfds(n){
}

PollManager::~PollManager() {
}

int PollManager::callPoll() {
	return (poll(&_sockets[0], nfds, -1));
}

pollfd &PollManager::operator[](int index) {
	return (_sockets[index]);
}

int PollManager::getNfds() {
	return (nfds);
}


void PollManager::addFd(size_t fd, short events) {
	_sockets[nfds].fd = fd;
	_sockets[nfds].events = events;
	_sockets[nfds].revents = 0;
	nfds++;
}

void PollManager::removeFd(size_t fd) {
	//1. find index of fd to remove
	size_t	i = 0;
	for (i = 0; i < nfds; i++)
		if (_sockets[i].fd == (int)fd)
			break ;
	//2. skip element and copy bacwards until the end
	if (i == nfds)
		return ;
	i++;
	while (i < nfds) {
		_sockets[i] = _sockets[i+1];
		i++;
	}
	_sockets[i].fd = 0;
	_sockets[i].events = 0;
	_sockets[i].revents = 0;
	nfds--;
}
