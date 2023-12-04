/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToString.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:04:23 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 13:11:12 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		TO_STRING_TPP
# define	TO_STRING_TPP

# include	<sstream>
# include	<string>

namespace http {
	template < typename T >
	std::string	toString(const T &toStringREF)
	{
		std::stringstream	ss;

		ss << toStringREF;
		return (ss.str());
	};
}

#endif