/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex_namespace.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:02 by jyao              #+#    #+#             */
/*   Updated: 2023/11/27 16:12:05 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		AUTOINDEX_HPP
# define	AUTOINDEX_HPP

# include	<string>

namespace	Autoindex {
	std::string	genPage(const char *path, const std::string &hostREF, const int &portREF);
};

#endif