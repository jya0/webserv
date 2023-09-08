/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/08 10:50:42 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig_namespace.hpp"


void printMap(DirectiveBlock *d1)
{
	std::string current_key = "";
	for (std::multimap<std::string, ADirective *>::const_iterator itr = d1->getDirectives()->begin(); itr != d1->getDirectives()->end(); ++itr)
	{
		if (current_key == itr->first)
		{
			continue;
		}
		else
		{
			current_key = itr->first;
		}
		std::pair< std::multimap< std::string, ADirective * >::const_iterator, std::multimap< std::string, ADirective * >::const_iterator> result = d1->getDirectives()->equal_range(itr->first);
		std::cout << itr->first << ": ";
		for (std::multimap<std::string, ADirective *>::const_iterator it = result.first; it != result.second; ++it)
		{
			if (dynamic_cast<DirectiveBlock *>(itr->second))
			{
				DirectiveBlock *b = dynamic_cast<DirectiveBlock *>(itr->second);
				printMap(b);
			}
		}
		std::cout << std::endl;
	}
}


// void printDirectiveBlock(Directive Block *b)
// {
	
// }

int main(void)
{
	HTTPServerParser::parseConfigFile1("../conf.d/test.conf");

	return (0);
}
