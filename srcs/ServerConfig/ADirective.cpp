/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 14:17:56 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"ADirective.hpp"
#include	"HTTPServerParser.hpp"

/**
 * @brief Construct a new ADirective::ADirective object
 *
 * Only has to initialise the _dveType enum DirectiveType object
 *
 */
ADirective::ADirective(void): _dveType(INIT)
{
}

/**
 * @brief Construct a new ADirective::ADirective object (copy constructor)
 * 
 * @param dveREF the reference to another ADirective object
 */
ADirective::ADirective(ADirective const	&dveREF)
{
	*this = dveREF;
}

/**
 * @brief Destroy the ADirective::ADirective object
 * 
 */
ADirective::~ADirective(void)
{
}

/**
 * @brief Assignment operator used to copy ADirective objects
 * 
 * @param dveREF the reference to another ADirective object
 * @return ADirective& 
 */
ADirective	&ADirective::operator=(ADirective const	&dveREF)
{
	_dveName = dveREF.getName();
	_dveValues = dveREF.getValues();
	// _dveType = dveREF.getType();
	return (*this);
}

/**
 * @brief getter for the _dveName attribute
 * 
 * @return std::string 
 */
std::string	ADirective::getName(void) const
{
	return (_dveName);
}

/**
 * @brief getter for the _dveValues attribute which is a vector of strings
 * 
 * @return std::vector< std::string > 
 */
std::vector< std::string >	ADirective::getValues(void) const
{
	return (_dveValues);
}

/**
 * @brief getter for the _dveType attribute
 * 
 * @return DirectiveType 
 */
DirectiveType	ADirective::getType(void) const
{
	return (_dveType);
}

/**
 * @brief setter for the _dveName attribute
 * 
 * @param strREF 
 */
void	ADirective::setName(std::string const	&strREF)
{
	_dveName = strREF;
}

/**
 * @brief setter for the vector of string value _dveValues attribute
 * 
 * @param strsREF 
 */
void	ADirective::setValues(std::vector< std::string > const	&strsREF)
{
	_dveValues = strsREF;
}

/**
 * @brief called recursively and polymorphically to print the whole directive (if it's a block directive).
 * 
 */
void	ADirective::printDirective(void) const
{
	std::cout << std::endl << _dveName << " ";
	for (std::vector< std::string >::const_iterator it = _dveValues.begin(); it != _dveValues.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

/**
 * @brief called recursively and polymorphically to parse the whole directive (if it's a block directive).
 * 
 * has basic checks for the structure of each directive but doesn't check the actual contents
 * 
 * the actual contents will be error checked with the following function call to scanDirective(void)
 * 
 */
int	ADirective::parseDirective(void)
{
	int											errorReturn;
	static int									nonSpaceLineNo;
	std::size_t									colonLoc;
	std::vector< std::string >::const_iterator	dveNameITR;

	errorReturn = 0;
	++nonSpaceLineNo;
	if (!errorReturn)
	{
		colonLoc = _dveName.find_first_of(":", 0);
		if (colonLoc == std::string::npos || (colonLoc + 1) != _dveName.length())
			std::cerr << "Error " << (errorReturn = 1) << " missing or wrong ':' at " << "\"" << _dveName << "\"" << std::endl;
	}
	if (!errorReturn)
	{
		dveNameITR = std::find(HTTPServerParser::dveNames.begin(), HTTPServerParser::dveNames.end(), _dveName);
		if (dveNameITR == HTTPServerParser::dveNames.end())
			std::cerr << "Error " << (errorReturn = 2) << " directive name: " << "\"" << _dveName << "\"" << std::endl;
	}
	if (!errorReturn)
	{
		_dveType = (DirectiveType)(dveNameITR - HTTPServerParser::dveNames.begin());
		if (((_dveType == SERVER) != _dveValues.empty())
			|| ((_dveType >= LIMIT_EXCEPT && _dveType <= SERVER) != (dynamic_cast< DirectiveSimple * >(this) == NULL)))
			std::cerr << "Error " << (errorReturn = 3) << " missing or extra directive values: " << "\"" << _dveName << "\"" << std::endl;
	}
	if (errorReturn)
		std::cerr << "Error at non-space line " << nonSpaceLineNo << std::endl;
	return (errorReturn);
}
