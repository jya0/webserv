/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/10/22 10:33:36 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<iostream>
# include	<vector>

//ALL DIRECTIVE NAMES!
/*SIMPLE*/
# define	DVE_AUTO_INDEX		"auto_index"			":"	//bool
# define	DVE_CGI_BIN			"cgi_bin"				":"	//std::string
# define	DVE_CMB_SIZE		"client_max_body_size"	":"	//std::size_t
# define	DVE_ERROR_PAGE		"error_page"			":"	//std::string
# define	DVE_INDEX			"index"					":"	//std::string
# define	DVE_LISTEN			"listen"				":"	//std::string
# define	DVE_RETURN			"return"				":"	//struct Return {int, std::string}
# define	DVE_ROOT			"root"					":"	//std::string
# define	DVE_SERVER_NAME		"server_name"			":"	//std::string
/*BLOCK*/
# define	DVE_LIMIT_EXECPT	"limit_except"			":"	//struct LimitExcept {enum HttpMethods}
# define	DVE_LOCATION		"location"				":"	//struct Location {}
# define	DVE_SERVER			"server"				":"	//class ServerConfig {}

# define	SIMPLE_DIRECTIVES	\
								DVE_AUTO_INDEX			" "		\
								DVE_CGI_BIN				" "		\
								DVE_CMB_SIZE			" "		\
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