/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveSimple.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:11:12 by jyao              #+#    #+#             */
/*   Updated: 2023/10/21 16:30:05 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"DirectiveSimple.hpp"

DirectiveSimple::DirectiveSimple(void)
{
}

DirectiveSimple::DirectiveSimple(DirectiveSimple const &simpleREF): ADirective(simpleREF)
{
}

DirectiveSimple::~DirectiveSimple(void)
{
}

DirectiveSimple	&DirectiveSimple::operator=(DirectiveSimple const &simpleREF)
{
	_dveName = simpleREF.getName();
	_dveValues = simpleREF.getValues();
	_dveType = simpleREF.getType();
	return (*this);
}
