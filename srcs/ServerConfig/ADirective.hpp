/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 15:14:06 by jyao             ###   ########.fr       */
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

/**
 * @brief Definitions for all directive's string names separated by 
 * SIMPLE_DIRECTIVES and BLOCK_DIRECTIVES
 * 
 */
# define	DVE_ALLOW			"allow"					":"
# define	DVE_AUTO_INDEX		"auto_index"			":"
# define	DVE_CGI_BIN			"cgi_bin"				":"
# define	DVE_CMB_SIZE		"client_max_body_size"	":"
# define	DVE_DENY			"deny"					":"
# define	DVE_ERROR_PAGE		"error_page"			":"
# define	DVE_INDEX			"index"					":"
# define	DVE_LISTEN			"listen"				":"
# define	DVE_RETURN			"return"				":"
# define	DVE_ROOT			"root"					":"
# define	DVE_SERVER_NAME		"server_name"			":"

# define	DVE_LIMIT_EXECPT	"limit_except"			":"
# define	DVE_LOCATION		"location"				":"
# define	DVE_SERVER			"server"				":"

# define	SIMPLE_DIRECTIVES	\
								DVE_ALLOW				" "		\
								DVE_AUTO_INDEX			" "		\
								DVE_CGI_BIN				" "		\
								DVE_CMB_SIZE			" "		\
								DVE_DENY				" "		\
								DVE_ERROR_PAGE			" "		\
								DVE_INDEX				" "		\
								DVE_LISTEN				" "		\
								DVE_RETURN				" "		\
								DVE_ROOT				" "		\
								DVE_SERVER_NAME			" "

# define	BLOCK_DIRECTIVES	\
								DVE_LIMIT_EXECPT		" "		\
								DVE_LOCATION			" "		\
								DVE_SERVER				" "

/**
 * @brief this enum is used to save the type of the parsed directive 
 * DirectiveBlock or DirectiveSimple
 * 
 */
typedef enum	DirectiveType {
	INIT = -1,
	ALLOW,
	AUTOINDEX,
	CGI_BIN,
	CLIENT_MAX_BODY_SIZE,
	DENY,
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
 * @method printDirective() can be called recursively and polymorphically to print
 * the whole directive (if it's a block directive).
 * @method parseDirective() is used recursively and 
 * polymorphically to parse the multimap 
 * gotten from getNextDirectiveBlock(std::ifstream &configIF)
 * 
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