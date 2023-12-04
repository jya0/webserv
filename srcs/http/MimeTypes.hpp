/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 02:15:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 03:44:27 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		MIME_TYPES_HPP
# define	MIME_TYPES_HPP

# include	<map>
# include	<vector>
# include	<utility>
# include	<string>
# include	<fstream>
# include	<iostream>
# include	<sstream>

# define	DEFAULT_MIME_FILE		"default_mimes.txt"
# define	MIME_FILE_DELIM			'\n'

static std::vector< std::pair< std::string, std::string > >	loadMimeFile(void) {
	std::vector< std::pair< std::string, std::string > >	mimePairs;
	std::ifstream		infile;
	std::stringstream	lineSS;
	std::string			line;
	std::string			key;
	std::string			value;

	infile.open(line.c_str(), std::ios::in);
	if (infile.good())
	{
		while (std::getline(infile, line, MIME_FILE_DELIM))
		{
			lineSS.clear();
			lineSS.str(line);
			lineSS >> value;
			lineSS >> key;
			mimePairs.push_back(std::make_pair(key, value));
		}
	}
	return (mimePairs);
};

namespace	http {
	namespace {
		std::vector< std::pair< std::string, std::string > >	mimePairs = loadMimeFile();
	}
	static const std::map< std::string, std::string >		mimeTypes(mimePairs.begin(), mimePairs.end());
	class IsMimeTypeUnary {
		private:
			std::string	_mimeType;
		protected:
		public:
			IsMimeTypeUnary(const std::string &mimeType): _mimeType(mimeType) {};
			bool	operator()(const std::pair< std::string, std::string > &mimePairREF)
			{
				return (mimePairREF.first.find(_mimeType, 0) != std::string::npos);
			};
	};
	std::string	checkMimeType(const std::string &keyREF);
}

#endif