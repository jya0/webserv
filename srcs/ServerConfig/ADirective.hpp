/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 13:13:49 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<string>
# include	<vector>
# include	"ServerConfig_namespace.hpp"

/**
 * @brief The abstract class inherited by DirectiveBlock and DirectiveSimple
 */
class	ADirective {
	protected:
		std::string	_dveName;	///used as the key for the map of Directive
		std::vector< std::string >	_dveValues;	///used as the key for the map of Directive
		serverConfig::DirectiveType	_dveType;	///enum DirectiveType is declared in the serverConfig namespace
	public:
		/*Canonical Class Form*/
		ADirective(void);
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		/*Accessors*/
		std::string	getName(void) const;
		std::vector< std::string >	getValues(void) const;
		serverConfig::DirectiveType	getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
};

#endif