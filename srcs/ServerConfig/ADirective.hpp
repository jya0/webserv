/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:50:31 by jyao              #+#    #+#             */
/*   Updated: 2023/12/08 23:47:32 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include	<iostream>
# include	<vector>

//ALL DIRECTIVE NAMES!
/*SIMPLE*/
# define	DVE_AUTO_INDEX		"auto_index"			":"	//bool
# define	DVE_CMB_SIZE		"client_max_body_size"	":"	//std::size_t
# define	DVE_ERROR_PAGE		"error_page"			":"	//std::string
# define	DVE_INDEX			"index"					":"	//std::string
# define	DVE_LIMIT_EXECPT	"limit_except"			":"	//struct LimitExcept {enum HttpMethods}
# define	DVE_LISTEN			"listen"				":"	//std::string
# define	DVE_RETURN			"return"				":"	//struct Return {int, std::string}
# define	DVE_ROOT			"root"					":"	//std::string
# define	DVE_SERVER_NAME		"server_name"			":"	//std::string
/*BLOCK*/
# define	DVE_LOCATION		"location"				":"	//struct Location {}
# define	DVE_SERVER			"server"				":"	//class ServerConfig {}

# define	SIMPLE_DIRECTIVES	\
								DVE_AUTO_INDEX			" "		\
								DVE_CMB_SIZE			" "		\
								DVE_ERROR_PAGE			" "		\
								DVE_INDEX				" "		\
								DVE_LIMIT_EXECPT		" "		\
								DVE_LISTEN				" "		\
								DVE_RETURN				" "		\
								DVE_ROOT				" "		\
								DVE_SERVER_NAME			" "

# define	BLOCK_DIRECTIVES	\
								DVE_LOCATION			" "		\
								DVE_SERVER				" "

enum	e_directiveType {
	INIT = -1,
	AUTOINDEX,
	CLIENT_MAX_BODY_SIZE,
	ERROR_PAGE,
	INDEX,
	LIMIT_EXCEPT,
	LISTEN,
	RETURN,
	ROOT,
	SERVER_NAME,
	LOCATION,
	SERVER,
	HTTP
};

/**
 * @brief The abstract class inherited by DirectiveBlock and DirectiveSimple
 *
 * @field _dveName	used as the key for the map of Directive
 * @field _dveValues used as the key for the map of Directive
 * @field _dveType enum e_directiveType is declared in the serverConfig namespace
 *
 * @method
 */
class ADirective {
	protected:
		std::string	_dveName;
		std::vector< std::string >	_dveValues;
		e_directiveType 			_dveType;
		ADirective(void);
	public:
		ADirective(ADirective const	&dveREF);
		virtual	~ADirective(void);
		ADirective	&operator=(ADirective const	&dveREF);

		const std::string					&getName(void) const;
		const std::vector< std::string >	&getValues(void) const;
		const e_directiveType				&getType(void) const;
		void	setName(std::string const	&strREF);
		void	setValues(std::vector< std::string > const	&strsREF);
		virtual void	printDirective(void) const;
		virtual int		parseDirective(void);
		virtual int		checkDirective(const e_directiveType &dveContextREF) const;
};

#endif