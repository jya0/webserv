/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:16:14 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 16:02:09 by jyao             ###   ########.fr       */
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

ADirective	*DirectiveBlock::findDirective(const std::string &dveName)
{
	std::multimap< std::string, ADirective * >::const_iterator	dveITR;

	dveITR = _dvesMap.find(dveName);
	if (dveITR == _dvesMap.end())
	{
		std::cerr << "Directive not found: " << dveName << "\n";
		return (NULL);
	}
	std::cout << "Found " << _dvesMap.count(dveName) << " " << dveName << std::endl;
	return (dveITR->second);
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
