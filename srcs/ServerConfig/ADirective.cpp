/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/11/30 14:28:47 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"ADirective.hpp"
#include	"HTTPServerParser.hpp"
#include	<algorithm>

/**
 * @brief Construct a new ADirective::ADirective object
 *
 * Only has to initialise the _dveType enum DirectiveType object
 *
 */
ADirective::ADirective(void): _dveType(INIT) {
}

ADirective::ADirective(ADirective const	&dveREF) {
	*this = dveREF;
}

ADirective::~ADirective(void) {
}

ADirective	&ADirective::operator=(ADirective const	&dveREF) {
	_dveName = dveREF.getName();
	_dveValues = dveREF.getValues();
	// _dveType = dveREF.getType();
	return (*this);
}

std::string	ADirective::getName(void) const {
	return (_dveName);
}

std::vector< std::string >	ADirective::getValues(void) const {
	return (_dveValues);
}

DirectiveType	ADirective::getType(void) const {
	return (_dveType);
}

void	ADirective::setName(std::string const	&strREF) {
	_dveName = strREF;
}

void	ADirective::setValues(std::vector< std::string > const	&strsREF) {
	_dveValues = strsREF;
}

void	ADirective::printDirective(void) const {
	std::cout << std::endl << _dveName << " ";
	for (std::vector< std::string >::const_iterator it = _dveValues.begin(); it != _dveValues.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

int	ADirective::parseDirective(void) {
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
			|| ((_dveType >= AUTOINDEX && _dveType <= SERVER_NAME) != (dynamic_cast<DirectiveBlock *>(this) == NULL)))
			std::cerr << "Error " << (errorReturn = 3) << " missing or extra directive values: " << "\"" << _dveName << "\"" << std::endl;
	}
	if (errorReturn)
		std::cerr << "Error at non-space line " << nonSpaceLineNo << std::endl;
	return (errorReturn);
}
