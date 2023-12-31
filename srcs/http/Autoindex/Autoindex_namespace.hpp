/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex_namespace.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:02 by jyao              #+#    #+#             */
/*   Updated: 2023/12/13 06:20:13 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		AUTOINDEX_HPP
# define	AUTOINDEX_HPP

# include	<string>
# include	"ServerConfig.hpp"
# include	"Request.hpp"

namespace	http {
	namespace	Autoindex {
		std::string	genPage(std::string path, const Request &requestREF);
		int			isPathFolder(const std::string &path);
		int			isPathReg(const std::string &path);
		int			isPathRead(const std::string &path);
		int			isPathWrite(const std::string &path);
		int			isPathExec(const std::string &path);
		int			isPathExist(const std::string &path);
	};
}

#endif