/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig_namespace.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:48:06 by jyao              #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/09/03 13:41:39 by jyao             ###   ########.fr       */
=======
/*   Updated: 2023/09/03 12:31:26 by rriyas           ###   ########.fr       */
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_NAMESPACE_HPP
# define SERVER_CONFIG_NAMESPACE_HPP

<<<<<<< HEAD
# include	"ADirective.hpp"
# include	"DirectiveSimple.hpp"
# include	"DirectiveBlock.hpp"

# define	SPACE_CHARSET	" \n\t\v\f\r"

=======
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
namespace	serverConfig {
	enum DirectiveType {
		INIT = -1,
		AUTOINDEX,
		CLIENT_MAX_BODY_SIZE,
		ERROR_PAGE,
		INDEX,
		LIMIT_EXCEPT,
		LISTEN,
		LOCATION,
		RETURN,
		ROOT,
		SERVER_NAME
	};
}

# include	"ADirective.hpp"
# include	"DirectiveSimple.hpp"
# include	"DirectiveBlock.hpp"

#endif