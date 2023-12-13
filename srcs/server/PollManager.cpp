/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 05:03:53 by rriyas            #+#    #+#             */
/*   Updated: 2023/12/13 19:16:55 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollManager.hpp"

PollManager::PollManager(size_t n) : nfds(n)
{
	memset(&_sockets, 0, MAX_SOCKETS * sizeof(struct pollfd));
}

PollManager::PollManager(const PollManager &rhs)
{
	*this = rhs;
}

PollManager &PollManager::operator=(const PollManager &rhs)
{
	if (this == &rhs)
		return (*this);
	this->nfds= rhs.nfds;
	for (size_t i = 0; i< nfds; i++)
		this->_sockets[i] = rhs._sockets[i];
	return (*this);
}


PollManager::~PollManager()
{
}

int PollManager::callPoll()
{
	return (poll(&_sockets[0], nfds, -1));
}

pollfd &PollManager::operator[](int index)
{
	return (_sockets[index]);
}

int PollManager::getNfds()
{
	return (nfds);
}

void PollManager::addFd(size_t fd, short events)
{
	_sockets[nfds].fd = fd;
	_sockets[nfds].events = events;
	_sockets[nfds].revents = 0;
	nfds++;
}

void PollManager::removeFd(size_t fd)
{
	size_t i = 0;
	for (i = 0; i < nfds; i++)
		if (_sockets[i].fd == (int)fd)
			break;
	if (i == nfds)
		return;
	i++;
	while (i < nfds)
	{
		_sockets[i] = _sockets[i + 1];
		i++;
	}
	_sockets[i].fd = 0;
	_sockets[i].events = 0;
	_sockets[i].revents = 0;
	nfds--;
}
