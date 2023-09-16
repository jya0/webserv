/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveSimple.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:11:12 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 15:11:12 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"DirectiveSimple.hpp"

/**
 * @brief Construct a new Directive Simple:: Directive Simple object
 * 
 */
DirectiveSimple::DirectiveSimple(void)
{
}

/**
 * @brief Construct a new Directive Simple:: Directive Simple object (copy constructor)
 * 
 * @param simpleREF 
 */
DirectiveSimple::DirectiveSimple(DirectiveSimple const &simpleREF): ADirective(simpleREF)
{
}

/**
 * @brief Destroy the Directive Simple:: Directive Simple object
 * 
 */
DirectiveSimple::~DirectiveSimple(void)
{
}

/**
 * @brief Assignment operator used to copy DirectiveSimple objects
 * 
 * @param simpleREF the reference to another DirectiveSimple object
 * @return DirectiveSimple& 
 */
DirectiveSimple	&DirectiveSimple::operator=(DirectiveSimple const &simpleREF)
{
	_dveName = simpleREF.getName();
	_dveValues = simpleREF.getValues();
	_dveType = simpleREF.getType();
	return (*this);
}
