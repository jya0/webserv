/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveSimple.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:15:28 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 14:34:36 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_SIMPLE_HPP
# define DIRECTIVE_SIMPLE_HPP

#include "ServerConfig_namespace.hpp"

/**
 * @brief Really just an empty class to extend ADirective so DirectiveBlock
 * class _dvesMap can point to either another DirectiveBlock or DirectiveSimple
 *
 */
class	DirectiveSimple : public ADirective {
	public:
		DirectiveSimple(void);
		DirectiveSimple(DirectiveSimple const &simpleREF);
		~DirectiveSimple(void);
		DirectiveSimple	&operator=(DirectiveSimple const &simpleREF);
};

#endif