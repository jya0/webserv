/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 13:00:45 by jyao             ###   ########.fr       */
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
		std::cout<<"Test 1 - server port: \n";
		serverBlocks.front()->findDirective(":");
	}
	return (0);
}
