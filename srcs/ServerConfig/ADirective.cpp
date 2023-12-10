/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 12:31:46 by jyao              #+#    #+#             */
/*   Updated: 2023/12/11 00:52:43 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<sstream>
#include	<algorithm>
#include	"ADirective.hpp"
#include	"Request.hpp"
#include	"ServerParser_namespace.hpp"

/**
 * @brief Construct a new ADirective::ADirective object
 *
 * Only has to initialise the _dveType enum e_directiveType object
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

const std::string	&ADirective::getName(void) const {
	return (_dveName);
}

const std::vector< std::string >	&ADirective::getValues(void) const {
	return (_dveValues);
}

const e_directiveType	&ADirective::getType(void) const {
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
	std::size_t									colonLoc;
	std::vector< std::string >::const_iterator	dveNameITR;
	std::stringstream							ss;

	ServerParser::loadLineNo++;
	colonLoc = _dveName.find_first_of(":", 0);
	if (colonLoc == std::string::npos || (colonLoc + 1) != _dveName.size())
	{
		ss << "Error " << ServerParser::ERROR_COLON << " missing or wrong ':' at " << "\"" << _dveName << "\"" << std::endl;
		throw (ServerParser::ParseErrorException(ss.str(), ServerParser::loadLineNo));
	}
	dveNameITR = std::find(ServerParser::dveNames.begin(), ServerParser::dveNames.end(), _dveName);
	if (dveNameITR == ServerParser::dveNames.end())
	{
		ss << "Error " << ServerParser::ERROR_DVE_NAME << " directive name: " << "\"" << _dveName << "\"" << std::endl;
		throw (ServerParser::ParseErrorException(ss.str(), ServerParser::loadLineNo));
	}
	_dveType = (e_directiveType)(dveNameITR - ServerParser::dveNames.begin());
	if (((_dveType == SERVER) != _dveValues.empty())
		|| ((_dveType >= AUTOINDEX && _dveType <= SERVER_NAME) != (dynamic_cast<DirectiveBlock *>(this) == NULL)))
	{
		ss << "Error " << ServerParser::ERROR_DVE_TYPE << " incompatible directive type: " << "\"" << _dveName << "\"" << std::endl;
		throw (ServerParser::ParseErrorException(ss.str(), ServerParser::loadLineNo));
	}
	return (0);
}

static bool	checkContext(const e_directiveType &dveContextREF, const e_directiveType &dveTypeREF)
{
	switch (dveTypeREF)
	{
		case (INIT):
			return (false);
		case (AUTOINDEX):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (CLIENT_MAX_BODY_SIZE):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (ERROR_PAGE):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (INDEX):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (LIMIT_EXCEPT):
			return (dveContextREF == LOCATION);
		case (LISTEN):
			return (dveContextREF == SERVER);
		case (RETURN):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (ROOT):
			return (dveContextREF == LOCATION || dveContextREF == SERVER);
		case (SERVER_NAME):
			return (dveContextREF == SERVER);
		case (LOCATION):
			return (dveContextREF == SERVER);
		case (SERVER):
			return (dveContextREF == HTTP);
		case (HTTP):
			return (false);
	}
	return (false);
}

static bool	isNumber(const std::string &strREF)
{
	std::stringstream	ss;
	int					test;

	ss.str(strREF);
	ss >> test;
	return (ss.good() && ss.rdbuf()->in_avail() == 0);
}

static bool isValidCode(const std::string &strREF)
{
	int	value;

	if (isNumber(strREF))
	{
		value = strtol(strREF.c_str(), NULL, 10);
		return (value >= 100 && value < 600);
	}
	return (false);
}

static bool	checkValues(const e_directiveType &dveTypeREF, const std::vector< std::string > &dveValuesREF)
{
	std::vector< std::string >::const_iterator	itc;

	switch (dveTypeREF)
	{
		case (INIT):
			return (false);
		case (AUTOINDEX):
		{
			return (dveValuesREF.size() == 1 && 
				(dveValuesREF.front() == "on" || dveValuesREF.front() == "off"));
		}
		case (CLIENT_MAX_BODY_SIZE):
		{
			return (dveValuesREF.size() == 1 && isNumber(dveValuesREF.front()) && std::atol(dveValuesREF.front().c_str()) > 0);
		}
		case (ERROR_PAGE):
		{
			if (isNumber(dveValuesREF.back()))
				return (false);
			for (itc = dveValuesREF.begin(); itc != dveValuesREF.end(); ++itc)
			{
				if (!isValidCode(*itc) && (itc + 1 != dveValuesREF.end()))
					return (false);
			}
			return (dveValuesREF.size() > 1);
		}
		case (INDEX):
		{
			return (dveValuesREF.size() >= 1);
		}
		case (LIMIT_EXCEPT):
		{
			for (itc = dveValuesREF.begin(); itc != dveValuesREF.end(); ++itc)
			{
				if (std::string(GET_METHOD "\n" HEAD_METHOD "\n" PUT_METHOD "\n" POST_METHOD "\n" DELETE_METHOD).find(*itc) == std::string::npos)
					return (false);
			}
			return (!dveValuesREF.empty() && itc == dveValuesREF.end());
		}
		case (LISTEN):
		{
			return (dveValuesREF.size() == 1 && std::count(dveValuesREF.front().begin(), dveValuesREF.front().end(), ':') == 1);
		}
		case (RETURN):
		{
			if (dveValuesREF.size() > 1 && !isValidCode(dveValuesREF.front()))
				return (false);
			return (dveValuesREF.size() == 1 || dveValuesREF.size() == 2);
		}
		case (ROOT):
		{
			return (dveValuesREF.size() == 1);
		}
		case (SERVER_NAME):
		{
			return (dveValuesREF.size() >= 1);
		}
		case (LOCATION):
		{
			return (dveValuesREF.size() == 1);
		}
		case (SERVER):
		{
			return (dveValuesREF.empty());
		}
		case (HTTP):
			return (false);
	}
	return (false);
}

int		ADirective::checkDirective(const e_directiveType &dveContextREF) const {
	std::stringstream	ss;

	ServerParser::checkLineNo++;
	if (checkContext(dveContextREF, _dveType) == false)
	{
		ss << "Error " << ServerParser::ERROR_BAD_CONTEXT << " directive is not under the right context: " << "\"" << _dveName << "\"" << std::endl;
		throw (ServerParser::ParseErrorException(ss.str(), ServerParser::checkLineNo));
	}
	if (checkValues(_dveType, _dveValues) == false)
	{
		ss << "Error " << ServerParser::ERROR_DVE_VALUE << " error directive value(s): " << "\"" << _dveName << "\"" << std::endl;
		throw (ServerParser::ParseErrorException(ss.str(), ServerParser::checkLineNo));
	}
	return (0);
};