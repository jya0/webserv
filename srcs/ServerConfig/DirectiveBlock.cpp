/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:16:14 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 15:19:04 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"DirectiveBlock.hpp"

/**
 * @brief Construct a new Directive Block:: Directive Block object
 * 
 */
DirectiveBlock::DirectiveBlock(void)
{
}

/**
 * @brief Construct a new Directive Block:: Directive Block object
 * 
 * @param blockREF the reference to another DirectiveBlock object
 */
DirectiveBlock::DirectiveBlock(DirectiveBlock const	&blockREF): ADirective(blockREF)
{
	*this = blockREF;
}

/**
 * @brief Destroy the Directive Block:: Directive Block object
 * 
 */
DirectiveBlock::~DirectiveBlock(void)
{
	_dvesMap.clear();
}

/**
 * @brief Assignment operator used to copy DirectiveBlock objects 
 * 
 * @param blockREF the reference to another DirectiveBlock object
 * @return DirectiveBlock& 
 */
DirectiveBlock	&DirectiveBlock::operator=(DirectiveBlock const	&blockREF)
{
	_dvesMap = *(blockREF.getDirectives());
	return (*this);
}

/**
 * @brief getter for the map of directives _dvesMap
 * 
 * @return const std::multimap<std::string, ADirective *>* 
 */
const std::multimap<std::string, ADirective *>	*DirectiveBlock::getDirectives(void) const
{
	return (&(_dvesMap));
}

/**
 * @brief takes any object inheriting ADirective which is mem alloc'd 
 * and adds it to the calling
 * block directive's _dvesMap
 * 
 * @param dvePTR a pointer to an object inheriting ADirective, 
 * it has to be mem alloc'd
 */
void	DirectiveBlock::insertMapDirective(ADirective *dvePTR)
{
	// std::pair< std::string, ADirective * > dvePair;

	// dvePair = std::make_pair(dvePTR->getName(), dvePTR);
	// std::multimap<std::string, ADirective *>	ma;
	std::pair<std::string, ADirective *> *p = 
		new std::pair<std::string, ADirective *>(dvePTR->getName(), dvePTR);
	_dvesMap.insert(*p);
}

/**
 * @brief returns the array of values from 
 * directive (the frist instance if there's duplicate)
 * 
 * @param dveName the name of the directive you want to get values for
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
		std::cout << "Found " << _dvesMap.count(dveName) << 
			" " << dveName << std::endl;
		std::cout << "Directive type: " << 
			dveITR->second->getType() << std::endl;
	}
	return (dveITR->second->getValues());
}

/**
 * @brief returns the DirectiveBlock pointer 
 * (the frist instance if there's duplicate block directives)
 * 
 * @param dveName the name of the directive you want to 
 * get the DirectiveBlock pointer for
 * @return DirectiveBlock* 
 */
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
		std::cout << "Found " << _dvesMap.count(dveName) << 
			" " << dveName << std::endl;
		std::cout << "Directive type: " << 
			dveITR->second->getType() << std::endl;
	}
	return (dynamic_cast< DirectiveBlock * >(dveITR->second));
}

/**
 * @brief returns the DirectiveBlock pointer 
 * (the frist instance if there's duplicate block directives with the same searchValue)
 * 
 * @param dveName the name of the directive you want to get the DirectiveBlock pointer for
 * @param searchValue the searchValue is used to find the 
 * directive block with the ADirective::_dveValues.front() equal to it
 * note: since ADirective::_dveValues is a vector of strings, in this 
 * function the searchValue is only compared to the first string of the vector
 * 
 * @return DirectiveBlock* 
 */
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
			std::cout << "value of directive block is " 
				<< loopITR->second->getValues().front() << std::endl;
			return (dynamic_cast< DirectiveBlock * >(loopITR->second));
		}
		// std::cout << loopITR->second->getValues().front() << ":" << searchValue << std::endl;
	}
	return (NULL);
}

/**
 * @brief this override of ADirective::printDirective(void) 
 * allows block directives to recursively print 
 * directives stored in its own _dvesMap
 * 
 */
void	DirectiveBlock::printDirective(void) const
{
	std::multimap< std::string, ADirective * >::const_iterator	it;

	this->ADirective::printDirective();
	for (it = _dvesMap.begin(); it != _dvesMap.end(); ++it)
	{
		it->second->printDirective();
	}
}

/**
 * @brief this override of ADirective::parseDirective(void) 
 * allows block directives to recursively parse directives 
 * stored in its own _dvesMap
 * 
 * it will stop its parsing loop of its directive in its _dvesMap
 * the moment an errorReturn occurs
 * 
 * @return int 
 */
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
