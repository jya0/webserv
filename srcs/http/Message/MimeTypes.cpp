/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 03:43:10 by jyao              #+#    #+#             */
/*   Updated: 2023/12/09 01:12:16 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<iostream>
#include	<sstream>
#include	"MimeTypes.hpp"
#include	<unistd.h>

/* std::string	http::checkMimeType(const std::string &uriREF) {
	http::t_mime_map::const_iterator	itc;
	std::stringstream					ss;
	std::string							suffix;

	ss.str(uriREF);
	std::getline(ss, suffix, '.');
	ss >> suffix;
	http::IsMimeTypeUnary	mimeUnary(suffix);
	itc = std::find_if(mimeTypes.begin(), mimeTypes.end(), mimeUnary);
	if (itc != mimeTypes.end())
		return (itc->second);
	return ("");
};
 */

std::string	http::checkMimeType(const std::string &uriREF) {
	http::t_mime_map::const_iterator	itc;
	size_t								lastDot;
	std::string							suffix;

	lastDot = uriREF.rfind('.');
	if (lastDot == std::string::npos || (lastDot + 1) >= uriREF.size())
		return ("");
	suffix = uriREF.substr(lastDot + 1, std::string::npos);
	itc = std::find_if(mimeTypes.begin(), mimeTypes.end(), http::IsMimeTypeUnary(suffix));
	if (itc != mimeTypes.end())
		return (itc->second);
	return ("");
};

http::t_mime_map	http::loadMimeFile(void) {
	t_mime_map					mimeTypes;
	std::ifstream				infile;
	std::stringstream			lineSS;
	std::string					line;
	std::string					key;
	std::vector< std::string >	keys;
	std::string					value;

	infile.open(DEFAULT_MIME_FILE, std::ios::in);
	if (infile.is_open())
	{
		while (std::getline(infile, line, MIME_FILE_DELIM))
		{
			lineSS.clear();
			lineSS.str(line);
			lineSS >> value;
			lineSS.clear();
			while (lineSS.rdbuf()->in_avail())
			{
				lineSS >> key;
				keys.push_back(key);
			}
			mimeTypes.push_back(std::make_pair(keys, value));
			keys.clear();
		}
	}
	return (mimeTypes);
};

namespace http
{
	const t_mime_map mimeTypes = loadMimeFile();
}

http::IsMimeTypeUnary::IsMimeTypeUnary(const std::string &mimeType): _mimeType(mimeType) {};

bool	http::IsMimeTypeUnary::operator()(const t_mime_pair &mimePairREF)
{
	// std::cerr << mimePairREF.second;
	for (std::vector< std::string >::const_iterator	itc = mimePairREF.first.begin(); itc != mimePairREF.first.end(); ++itc)
	{
		if (*itc == _mimeType)
			return (true);
	}
	return (false);
};