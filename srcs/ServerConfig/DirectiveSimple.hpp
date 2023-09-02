/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveSimple.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:15:28 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 12:28:06 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_SIMPLE_HPP
# define DIRECTIVE_SIMPLE_HPP

# include	"ADirective.hpp"

class	DirectiveSimple : private ADirective {
	public:
		DirectiveSimple(void);
		DirectiveSimple(DirectiveSimple const &simpleREF);
		virtual	~DirectiveSimple(void);
		DirectiveSimple	&operator=(DirectiveSimple const &simpleREF);
};

#endif