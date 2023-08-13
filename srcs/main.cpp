/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:38:21 by rriyas            #+#    #+#             */
/*   Updated: 2023/08/13 18:59:04 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	TCPServer server = TCPServer("127.0.0.1", 8080);
	server.startListening();
	return (0);
}