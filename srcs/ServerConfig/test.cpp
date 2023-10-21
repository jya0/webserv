/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/10/21 16:29:45 by jyao             ###   ########.fr       */
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
		std::cout << "-----------Accessing simple directives-----------" << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_AUTO_INDEX)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_CGI_BIN)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_CMB_SIZE)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_ERROR_PAGE)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_INDEX)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_LISTEN)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_RETURN)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_ROOT)[0] << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveSimple(DVE_SERVER_NAME)[0] << std::endl;
	}
	{
		std::cout << "-----------Accessing block directives-----------" << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveBlock(DVE_LOCATION)->getValues()[0] << std::endl;
		// std::cout << serverBlocks[0]->checkDirectiveBlock(DVE_LOCATION, "/bin")->checkDirectiveSimple(DVE_RETURN) << std::endl;
		std::cout << serverBlocks[0]->checkDirectiveBlock(DVE_LIMIT_EXECPT)->getValues()[0] << std::endl;
	}
	for (std::vector< DirectiveBlock * >::const_iterator iter = serverBlocks.begin(); iter != serverBlocks.end(); ++iter)
	{
		delete (*iter);
	}
	return (0);
}
