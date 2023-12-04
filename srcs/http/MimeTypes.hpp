/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 02:15:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 05:50:49 by jyao             ###   ########.fr       */
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

# define	DEFAULT_MIME_FILE		"./srcs/http/default_mimes.txt"
# define	MIME_FILE_DELIM			'\n'

namespace	http {
	extern const std::map< std::string, std::string >	mimeTypes;
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
	std::string	checkMimeType(const std::string &uriREF);
	std::map< std::string, std::string >	loadMimeFile(void);
}

#endif