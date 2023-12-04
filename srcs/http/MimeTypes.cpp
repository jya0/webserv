/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 03:43:10 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 03:43:46 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"MimeTypes.hpp"

std::string	http::checkMimeType(const std::string &keyREF) {
	std::map< std::string, std::string >::const_iterator	itc;

	itc = std::find_if(mimeTypes.begin(), mimeTypes.end(), IsMimeTypeUnary(keyREF));
	if (itc != mimeTypes.end())	
		return (itc->second);
	return ("");
};
