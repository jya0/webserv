/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 12:11:48 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<iostream>
# include	<utility>
# include	<string>
# include	<sstream>
# include	<fstream>
# include	<istream>
# include	<string>
# include	<vector>

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

typedef enum	DirectiveType {
	INIT = -1,
	AUTOINDEX,
	CGI_BIN,
	CLIENT_MAX_BODY_SIZE,
	ERROR_PAGE,
	INDEX,
	LISTEN,
	RETURN,
	ROOT,
	SERVER_NAME,
	LIMIT_EXCEPT,
	LOCATION,
	SERVER,
}	DirectiveType;

/**
 * @brief The abstract class inherited by DirectiveBlock and DirectiveSimple
 *
 * @field _dveName	used as the key for the map of Directive
 * @field _dveValues used as the key for the map of Directive
 * @field _dveType enum DirectiveType is declared in the serverConfig namespace
 *
 * @method
 */
class ADirective {
	protected:
		std::string	_dveName;
		std::vector< std::string >	_dveValues;
		DirectiveType 				_dveType;
		ADirective(void);
	public:
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		std::string	getName(void) const;
		std::vector< std::string >	getValues(void) const;
		DirectiveType				getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
		virtual void	printDirective(void) const;
		virtual int		parseDirective(void);
};

#endif