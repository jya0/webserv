/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/11/27 16:54:20 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<fstream>
#include	<sstream>
#include	<cstdio>
#include	<iostream>
#include	"Autoindex_namespace.hpp"

#define		TEST_HTML	"test.html"

int main(void)
{
	std::ofstream		ofile;
	// std::stringstream	ss;
	std::string			page;

	int			testPort = 5500;
	std::string	testHost = "127.0.0.1";

	page = Autoindex::genPage("../..", testHost, testPort);
	// ss.str(page);
	// std::cout << page;
	ofile.open(TEST_HTML, std::fstream::out);
	if (ofile.is_open())
		ofile << page;
	else
		std::cerr << "couldn't make file!"
			<< std::endl;
	ofile.close();
	std::getchar();
	std::remove(TEST_HTML);
	return (0);
}
