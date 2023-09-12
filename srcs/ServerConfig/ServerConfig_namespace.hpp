/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig_namespace.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:48:06 by jyao              #+#    #+#             */
/*   Updated: 2023/09/12 15:54:51 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_NAMESPACE_HPP
# define SERVER_CONFIG_NAMESPACE_HPP

# define	SPACE_CHARSET	" \n\t\v\f\r"

namespace	serverConfig {
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
		SERVER
	}	DirectiveType;
}

# include	"ADirective.hpp"
# include	"DirectiveSimple.hpp"
# include	"DirectiveBlock.hpp"
# include 	"HTTPServerParser.hpp"

#endif