/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 12:19:37 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServerParser.hpp"

int main(void)
{
	std::cout << SIMPLE_DIRECTIVES BLOCK_DIRECTIVES << std::endl;
	HTTPServerParser::parseConfigFile("../conf.d/test.conf");

	//Test Cases:
	{
		std::cout<<"Test 1 - server port: \n";
		
	}
	return (0);
}
