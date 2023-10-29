/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:16:14 by jyao              #+#    #+#             */
/*   Updated: 2023/10/21 16:45:48 by jyao             ###   ########.fr       */
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

void	DirectiveBlock::clearDvesMap(void)
{
	std::multimap<std::string, ADirective *>::const_iterator iter;

	for (iter = _dvesMap.begin(); iter != _dvesMap.end(); ++iter)
	{
		delete (iter->second);
	}
	// _dvesMap.clear();
	// delete (_dvesMap.begin()->second);
}

DirectiveBlock::~DirectiveBlock(void)
{
	clearDvesMap();
}

DirectiveBlock	&DirectiveBlock::operator=(DirectiveBlock const	&blockREF)
{
	clearDvesMap();
	_dvesMap = *(blockREF.getDvesMap());
	return (*this);
}

const std::multimap<std::string, ADirective *>	*DirectiveBlock::getDvesMap(void) const
{
	return (&(_dvesMap));
}

void	DirectiveBlock::insertMapDirective(ADirective *dvePTR)
{
	std::pair< std::string, ADirective * > dvePair;

	if (dvePTR == NULL)
		return ;
	dvePair = std::make_pair(dvePTR->getName(), dvePTR);
	// std::multimap<std::string, ADirective *>	ma;
	// std::pair<std::string, ADirective *> *p = new std::pair<std::string, ADirective *>(dvePTR->getName(), dvePTR);
	_dvesMap.insert(dvePair);
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
		return (std::vector< std::string >());
	}
	else
		std::cout << "Found " << _dvesMap.count(dveName) << " " << dveName << std::endl;
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
		std::cout << "Found " << _dvesMap.count(dveName) << " " << dveName << std::endl;
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
		if (loopITR->second->getValues().front().compare(searchValue))
			return (dynamic_cast< DirectiveBlock * >(loopITR->second));
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
