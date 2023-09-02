/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig_namespace.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 09:48:06 by jyao              #+#    #+#             */
/*   Updated: 2023/09/02 13:03:21 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_NAMESPACE_HPP
# define SERVER_CONFIG_NAMESPACE_HPP

# include	"ADirective.hpp"
# include	"DirectiveSimple.hpp"
# include	"DirectiveBlock.hpp"

namespace	serverConfig {
	enum class	DirectiveType {
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

#endif