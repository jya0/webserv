/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 02:15:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 11:01:41 by jyao             ###   ########.fr       */
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
	typedef	std::map< std::vector< std::string >, std::string >	t_mime_struct;
	extern const t_mime_struct	mimeTypes;
	class IsMimeTypeUnary {
		private:
			std::string	_mimeType;
		protected:
		public:
			IsMimeTypeUnary(const std::string &mimeType): _mimeType(mimeType) {};
			bool	operator()(const std::pair< std::vector< std::string >, std::string > &mimePairREF)
			{
				for (std::vector< std::string >::const_iterator	itc = mimePairREF.first.begin(); itc != mimePairREF.first.end(); ++itc)
				{
					if (*itc == _mimeType)
						return (true);
				}
				return (false);
			};
	};
	std::string	checkMimeType(const std::string &uriREF);
	t_mime_struct	loadMimeFile(void);
}

#endif