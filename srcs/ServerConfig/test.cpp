/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 22:07:41 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServerParser.hpp"

int main(void)
{
	std::vector< DirectiveBlock * >	serverBlocks;

	std::cout << SIMPLE_DIRECTIVES BLOCK_DIRECTIVES << std::endl;
	serverBlocks = HTTPServerParser::parseConfigFile("../conf.d/test.conf");

	//Test Cases:
	{
		std::cout << "Test 1 - server port: " << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_LISTEN)[0] << std::endl;
	}
	return (0);
}
