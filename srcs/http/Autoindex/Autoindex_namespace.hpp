/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex_namespace.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:02 by jyao              #+#    #+#             */
/*   Updated: 2023/12/01 14:21:57 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		AUTOINDEX_HPP
# define	AUTOINDEX_HPP

# include	<string>

namespace	http {
	namespace	Autoindex {
		std::string	genPage(const char *path, const std::string &hostREF, const int &portREF);
		int			isPathFolder(const std::string &path);
		int			isPathReg(const std::string &path);
	};
}

#endif