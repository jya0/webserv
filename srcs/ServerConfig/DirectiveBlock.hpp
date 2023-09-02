/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:53:05 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 12:27:47 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_BLOCK_HPP
# define DIRECTIVE_BLOCK_HPP

# include	<map>
# include	"ADirective.hpp"

class	DirectiveBlock : public ADirective {
	protected:
		std::multimap<std::string, ADirective *>	_dvesMap;
	public:
		DirectiveBlock(void);
		DirectiveBlock(DirectiveBlock const	&blockREF);
		~DirectiveBlock(void);
		DirectiveBlock	&operator=(DirectiveBlock const	&blockREF);

		std::multimap<std::string, ADirective *>	&getDirectives(void) const;
};

#endif