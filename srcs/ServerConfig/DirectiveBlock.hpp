/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:53:05 by jyao              #+#    #+#             */
/*   Updated: 2023/09/08 11:58:04 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_BLOCK_HPP
# define DIRECTIVE_BLOCK_HPP

# include	<map>
# include "ServerConfig_namespace.hpp"

/**
 * @brief DirectiveBlock has a map which can contain more Directives
 *
 * @field _dvesMap a multimap used to store all the directives, block or simple
 *
 */
class	DirectiveBlock : public ADirective {
	protected:
		std::multimap<std::string, ADirective *>	_dvesMap;
	public:
		DirectiveBlock(void);
		DirectiveBlock(DirectiveBlock const	&blockREF);
		~DirectiveBlock(void);
		DirectiveBlock	&operator=(DirectiveBlock const	&blockREF);

		void	insertMapDirective(ADirective *dvePTR);
		const std::multimap<std::string, ADirective *>	*getDirectives(void) const;
		std::pair< std::multimap< std::string, ADirective * >::iterator, std::multimap< std::string, ADirective * >::iterator> \
			operator[](std::string &key);
		void	printDirective(void) const;
};

#endif