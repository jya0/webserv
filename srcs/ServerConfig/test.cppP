/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/05 21:33:48 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerParser_namespace.hpp"

int main(void) {
	std::vector< DirectiveBlock * >	serverBlocks;

	std::cout << SIMPLE_DIRECTIVES BLOCK_DIRECTIVES << std::endl;
	serverBlocks = ServerParser::parseConfigFile("../conf.d/test.conf");

	//Test Cases:
	{
		std::cout << "-----------Accessing simple directives-----------" << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_AUTO_INDEX)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_CGI_PATH_INFO)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_CMB_SIZE)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_ERROR_PAGE)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_INDEX)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_LISTEN)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_RETURN)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_ROOT)[0] << std::endl;
		std::cout << serverBlocks[0]->readDirectiveSimple(DVE_SERVER_NAME)[0] << std::endl;
	}
	{
		std::cout << "-----------Accessing block directives-----------" << std::endl;
		std::cout << serverBlocks[0]->readDirectiveBlock(DVE_LOCATION)->getValues()[0] << std::endl;
		// std::cout << serverBlocks[0]->readDirectiveBlock(DVE_LOCATION, "/bin")->readDirectiveSimple(DVE_RETURN) << std::endl;
		std::cout << serverBlocks[0]->readDirectiveBlock(DVE_LIMIT_EXECPT)->getValues()[0] << std::endl;
	}
	for (std::vector< DirectiveBlock * >::const_iterator iter = serverBlocks.begin(); iter != serverBlocks.end(); ++iter)
	{
		delete (*iter);
	}
	return (0);
}
