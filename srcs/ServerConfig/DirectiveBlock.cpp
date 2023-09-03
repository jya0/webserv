/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:16:14 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 12:18:22 by rriyas           ###   ########.fr       */
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
}

DirectiveBlock	&DirectiveBlock::operator=(DirectiveBlock const	&blockREF)
{
	_dvesMap = *(blockREF.getDirectives());
	return (*this);
}

std::multimap<std::string, ADirective *>	*DirectiveBlock::getDirectives(void) const
{
	return (&(_dvesMap));
}

std::pair< std::map< std::string, ADirective * >::iterator, std::map< std::string, ADirective * >::iterator> *\
	DirectiveBlock::operator[](std::string &key)
{
	if (_dvesMap.find(key) == _dvesMap.end())
		return (NULL);
	return (&(_dvesMap.equal_range(key)));
}
