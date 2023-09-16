/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:53:05 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 14:39:33 by jyao             ###   ########.fr       */
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
 * @method insertMapDirective(ADirective *dvePTR)
 * takes any object inheriting ADirective which is mem alloc'd and adds it to the calling
 * block directive's _dvesMap
 * 
 * @method checkDirectiveSimple(const std::string &dveName)
 * should only be used to get the string values of simple directives
 * 
 * @method checkDirectiveBlock(const std::string &dveName, const std::string &searchValue)
 * is the 1 overload used to find a block directive with the specific searchValue
 * 
 * @method checkDirectiveBlock(const std::string &dveName)
 * is the 2 overload used to find the first instance of a block directive (if there are multiple)
 * 
 * @method parseDirective(void) is a polymorphic override of the version from ADirective
 * this override allows block directives to recursively parse directives stored in its own _dvesMap
 * 
 * @method printDirective(void) is a polymorphic override of the version from ADirective
 * this override allows block directives to recursively print directives stored in its own _dvesMap
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
		DirectiveBlock	*checkDirectiveBlock(const std::string &dveName, const std::string &searchValue);
		DirectiveBlock	*checkDirectiveBlock(const std::string &dveName);
		void	printDirective(void) const;
		int		parseDirective(void);
};

#endif