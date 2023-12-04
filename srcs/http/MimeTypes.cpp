/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 03:43:10 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 05:51:10 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<iostream>
#include	<sstream>
#include	"MimeTypes.hpp"

std::string	http::checkMimeType(const std::string &uriREF) {
	std::map< std::string, std::string >::const_iterator	itc;
	std::stringstream										ss;
	std::string												suffix;

	ss.str(uriREF);
	std::getline(ss, suffix, '.');
	ss >> suffix;
	itc = std::find_if(mimeTypes.begin(), mimeTypes.end(), IsMimeTypeUnary(suffix));
	if (itc != mimeTypes.end())	
		return (itc->second);
	return ("");
};

std::map< std::string, std::string >	http::loadMimeFile(void) {
	std::map< std::string, std::string > mimeTypes;
	std::ifstream		infile;
	std::stringstream	lineSS;
	std::string			line;
	std::string			key;
	std::string			value;

	infile.open(DEFAULT_MIME_FILE, std::ios::in);
	if (infile.good())
	{
		while (std::getline(infile, line, MIME_FILE_DELIM))
		{
			lineSS.clear();
			lineSS.str(line);
			lineSS >> value;
			lineSS >> key;
			mimeTypes.insert(std::make_pair(key, value));
		}
	}
	return (mimeTypes);
};

namespace http {
	const std::map< std::string, std::string >	mimeTypes = loadMimeFile();
}