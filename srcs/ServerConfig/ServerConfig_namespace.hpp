/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig_namespace.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:48:06 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 12:31:26 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_NAMESPACE_HPP
# define SERVER_CONFIG_NAMESPACE_HPP

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