/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:16:14 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 11:21:01 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"DirectiveBlock.hpp"

DirectiveBlock::DirectiveBlock(void)
{
}

DirectiveBlock::DirectiveBlock(DirectiveBlock const	&blockREF): ADirective(blockREF)
{
	*this = blockREF;
}

DirectiveBlock::~DirectiveBlock(void)
{
	_dvesMap.clear();
}

DirectiveBlock	&DirectiveBlock::operator=(DirectiveBlock const	&blockREF)
{
	_dvesMap = *(blockREF.getDirectives());
	return (*this);
}

const std::multimap<std::string, ADirective *>	*DirectiveBlock::getDirectives(void) const
{
	return (&(_dvesMap));
}

void	DirectiveBlock::insertMapDirective(ADirective *dvePTR)
{
	// std::pair< std::string, ADirective * > dvePair;

	// dvePair = std::make_pair(dvePTR->getName(), dvePTR);
	// std::multimap<std::string, ADirective *>	ma;
	std::pair<std::string, ADirective *> *p = new std::pair<std::string, ADirective *>(dvePTR->getName(), dvePTR);
	_dvesMap.insert(*p);
}

/**
 * @brief returns the array of values from directive (the frist instance if there's duplicate)
 * 
 * @param dveName 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	DirectiveBlock::checkDirectiveSimple(const std::string &dveName)
{
	std::multimap< std::string, ADirective * >::const_iterator	dveITR;

	dveITR = _dvesMap.find(dveName);
	if (dveITR == _dvesMap.end())
	{
		std::cerr << "Directive not found: " << dveName << "\n";
		return (std::vector< std::string >(1, "EMPTY"));
	}
	else
	{
		std::cout << "Found " << _dvesMap.count(dveName) << " " << dveName << std::endl;
		std::cout << "Directive type: " << dveITR->second->getType() << std::endl;
	}
	return (dveITR->second->getValues());
}

DirectiveBlock	*DirectiveBlock::checkDirectiveBlock(const std::string &dveName)
{
	std::multimap< std::string, ADirective * >::const_iterator	dveITR;

	dveITR = _dvesMap.find(dveName);
	if (dveITR == _dvesMap.end())
	{
		std::cerr << "Directive not found: " << dveName << "\n";
		return (NULL);
	}
	else
	{
		std::cout << "Found " << _dvesMap.count(dveName) << " " << dveName << std::endl;
		std::cout << "Directive type: " << dveITR->second->getType() << std::endl;
	}
	return (dynamic_cast< DirectiveBlock * >(dveITR->second));
}

DirectiveBlock	*DirectiveBlock::checkDirectiveBlock(const std::string &dveName, const std::string &searchValue)
{
	std::multimap< std::string, ADirective * >::const_iterator	loopITR;
	std::pair< std::multimap< std::string, ADirective * >::const_iterator, std::multimap< std::string, ADirective * >::const_iterator>	dveITRS;

	if (_dvesMap.count(dveName) == 0)
		return (NULL);
	dveITRS = _dvesMap.equal_range(dveName);
	for (loopITR = dveITRS.first; loopITR != dveITRS.second; ++loopITR)
	{
		if (loopITR->second->getValues().front().compare(searchValue) == 0)
		{
			std::cout << "value of directive block is " << loopITR->second->getValues().front() << std::endl;
			return (dynamic_cast< DirectiveBlock * >(loopITR->second));
		}
		// std::cout << loopITR->second->getValues().front() << ":" << searchValue << std::endl;
	}
	return (NULL);
}

void	DirectiveBlock::printDirective(void) const
{
	std::multimap< std::string, ADirective * >::const_iterator	it;

	this->ADirective::printDirective();
	for (it = _dvesMap.begin(); it != _dvesMap.end(); ++it)
	{
		it->second->printDirective();
	}
}

int	DirectiveBlock::parseDirective(void)
{
	int															errorReturn;
	std::multimap< std::string, ADirective * >::const_iterator	it;

	errorReturn = this->ADirective::parseDirective();
	for (it = _dvesMap.begin(); (it != _dvesMap.end() && !errorReturn); ++it)
	{
		errorReturn = it->second->parseDirective();
	}
	return (errorReturn);
}
