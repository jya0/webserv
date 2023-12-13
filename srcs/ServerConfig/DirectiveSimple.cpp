/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveSimple.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:11:12 by jyao              #+#    #+#             */
/*   Updated: 2023/12/13 04:03:49 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"DirectiveSimple.hpp"

DirectiveSimple::DirectiveSimple(void) {
}

DirectiveSimple::DirectiveSimple(DirectiveSimple const &simpleREF): ADirective(simpleREF) {
}

DirectiveSimple::~DirectiveSimple(void) {
}

DirectiveSimple	&DirectiveSimple::operator=(DirectiveSimple const &simpleREF) {
	this->ADirective::operator=(simpleREF);
	return (*this);
}
