/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 03:43:10 by jyao              #+#    #+#             */
/*   Updated: 2023/12/07 17:45:13 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<iostream>
#include	<sstream>
#include	"MimeTypes.hpp"
#include <unistd.h>

std::string	http::checkMimeType(const std::string &uriREF) {
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

http::t_mime_map	http::loadMimeFile(void) {
	t_mime_map					mimeTypes;
	std::ifstream				infile;
	std::stringstream			lineSS;
	std::string					line;
	std::string					key;
	std::vector< std::string >	keys;
	std::string					value;

	char temp[1024];
	std::string dir = getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string("");
	std::cerr<<dir;
	std::cerr<<DEFAULT_MIME_FILE<<std::endl;
	infile.open(DEFAULT_MIME_FILE, std::ios::in);
	std::cerr << infile.rdstate() << std::endl;
	if (infile.is_open())
	{
		std::cerr<<"IT OPENED!****************************\n";
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
	else
		std::cerr<<"NUUUUuuuuuuuuuuuuuuuu!****************************\n";
	return (mimeTypes);
};

namespace http
{
	const t_mime_map mimeTypes = loadMimeFile();

}

http::IsMimeTypeUnary::IsMimeTypeUnary(const std::string &mimeType): _mimeType(mimeType) {};

bool	http::IsMimeTypeUnary::operator()(const t_mime_pair &mimePairREF)
{
	// std::cerr << "I am called! MIME!\n";
	std::cerr << mimePairREF.second;
	for (std::vector< std::string >::const_iterator	itc = mimePairREF.first.begin(); itc != mimePairREF.first.end(); ++itc)
	{
		if (*itc == _mimeType)
			return (true);
	}
	return (false);
};