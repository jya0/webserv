/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:53:05 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 21:41:49 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_BLOCK_HPP
# define DIRECTIVE_BLOCK_HPP

# include	<map>
# include	"ADirective.hpp"

/**
 * @brief DirectiveBlock has a map which can contain more Directives
 *
 * @field _dvesMap a multimap used to store all the directives, block or simple
 *
 */
class DirectiveBlock : public ADirective {
	protected:
		std::multimap<std::string, ADirective *>	_dvesMap;
	public:
		DirectiveBlock(void);
		DirectiveBlock(DirectiveBlock const	&blockREF);
		~DirectiveBlock(void);
		DirectiveBlock	&operator=(DirectiveBlock const	&blockREF);

		void	insertMapDirective(ADirective *dvePTR);
		const std::multimap<std::string, ADirective *>	*getDirectives(void) const;
		std::vector<std::string>	checkDirectiveSimple(const std::string &dveName);
		DirectiveBlock	*checkDirectiveBlock(const std::string &dveName, const std::string &dveValue);
		DirectiveBlock	*checkDirectiveBlock(const std::string &dveName);
/* 		std::pair< std::multimap< std::string, ADirective * >::iterator, std::multimap< std::string, ADirective * >::iterator> \
			operator[](const std::string &key); */
		void	printDirective(void) const;
		int		parseDirective(void);
};

#endif