/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/09/12 16:10:25 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig_namespace.hpp"
#include	"ADirective.hpp"

/**
 * @brief Construct a new ADirective::ADirective object
 *
 * Only has to initialise the _dveType enum DirectiveType object
 *
 */
ADirective::ADirective(void): _dveType(serverConfig::INIT)
{
}

ADirective::ADirective(ADirective const	&dveREF)
{
	*this = dveREF;
}

ADirective::~ADirective(void)
{
}

ADirective	&ADirective::operator=(ADirective const	&dveREF)
{
	_dveName = dveREF.getName();
	_dveValues = dveREF.getValues();
	// _dveType = dveREF.getType();
	return (*this);
}

std::string	ADirective::getName(void) const
{
	return (_dveName);
}

std::vector< std::string >	ADirective::getValues(void) const
{
	return (_dveValues);
}

serverConfig::DirectiveType	ADirective::getType(void) const
{
	return (_dveType);
}

void	ADirective::setName(std::string const	&strREF)
{
	_dveName = strREF;
}

void	ADirective::setValues(std::vector< std::string > const	&strsREF)
{
	_dveValues = strsREF;
}

void	ADirective::printDirective(void) const
{
	std::cout << std::endl << _dveName << " ";
	for (std::vector< std::string >::const_iterator it = _dveValues.begin(); it != _dveValues.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

int	ADirective::checkDirective(void) const
{
	static int					nonSpaceLineNo;
	std::size_t					colonLoc;

	++nonSpaceLineNo;
	colonLoc = _dveName.find_first_of(":", 0);
	if (colonLoc == std::string::npos)
	{
		std::cerr << "Error ':' at " << "\"" << _dveName << "\"" << std::endl;
		std::cerr << "Error at non-space line " << nonSpaceLineNo << std::endl;
		return (-1);
	}
	return (0);
}