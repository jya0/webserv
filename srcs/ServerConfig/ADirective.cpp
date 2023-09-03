/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 12:23:30 by rriyas           ###   ########.fr       */
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
ADirective::ADirective(void): _dveType(serverConfig::DirectiveType::INIT)
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
	_dveType = dveREF.getType();
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
