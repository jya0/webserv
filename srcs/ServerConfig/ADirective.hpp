/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 23:09:22 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<string>
# include	<vector>
# include	"ServerConfig_namespace.hpp"
# include	"ErrorInfo.hpp"

/**
 * @brief The abstract class inherited by DirectiveBlock and DirectiveSimple
 *
 * @field _dveName	used as the key for the map of Directive
 * @field _dveValues used as the key for the map of Directive
 * @field _dveType enum DirectiveType is declared in the serverConfig namespace
 *
 * @method
 */
class	ADirective {
	protected:
		std::string	_dveName;
		std::vector< std::string >	_dveValues;
		serverConfig::DirectiveType	_dveType;
		ErrorInfo					_errorInfo;
	public:
		ADirective(void);
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		std::string	getName(void) const;
		std::vector< std::string >	getValues(void) const;
		// serverConfig::DirectiveType	getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
};

#endif