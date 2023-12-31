/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:53:05 by jyao              #+#    #+#             */
/*   Updated: 2023/12/08 23:43:24 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_BLOCK_HPP
# define DIRECTIVE_BLOCK_HPP

# include	<map>
# include	<exception>
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

		void	clearDvesMap(void);
		void	insertMapDirective(ADirective *dvePTR);
		const std::multimap<std::string, ADirective *>	*getDvesMap(void) const;
		std::vector< std::string >					readDirectiveSimple(const std::string &dveName) const;
		std::vector< std::vector< std::string > >	readDirectivesSimple(const std::string &dveName) const;
		DirectiveBlock								*readDirectiveBlock(const std::string &dveName, const std::string &dveValue) const;
		DirectiveBlock								*readDirectiveBlock(const std::string &dveName) const;
		void	printDirective(void) const;
		int		parseDirective(void);
		int		checkDirective(const e_directiveType &dveContextREF) const;
};

#endif