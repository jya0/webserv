/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 13:14:30 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
