/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/12 15:55:27 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<string>
# include	<vector>
# include	"ErrorInfo.hpp"
# include	"ServerConfig_namespace.hpp"


# define	SIMPLE_DIRECTIVES	\
								" auto_index "				\
								" cgi_bin "					\
								" client_max_body_size "	\
								" error_page "				\
								" index "					\
								" listen "					\
								" return "					\
								" root "					\
								" server_name "				\

# define	BLOCK_DIRECTIVES	\
								" limit_except "			\
								" location "				\
								" server "

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
		serverConfig::DirectiveType _dveType;
		ErrorInfo					_errorInfo;
		ADirective(void);
	public:
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		std::string	getName(void) const;
		std::vector< std::string >	getValues(void) const;
		serverConfig::DirectiveType	getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
		virtual void	printDirective(void) const;
		virtual int		checkDirective(void) const;
};

#endif