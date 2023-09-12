/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:20:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/12 10:49:55 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig_namespace.hpp"

int main(void)
{
	std::cout << SIMPLE_DIRECTIVES;
	HTTPServerParser::parseConfigFile("../conf.d/test.conf");

	return (0);
}
