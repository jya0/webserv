/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 02:15:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/11 17:13:25 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		MIME_TYPES_HPP
# define	MIME_TYPES_HPP

# include	<vector>
# include	<utility>
# include	<string>
# include	<fstream>
# include	<iostream>
# include	<sstream>
# include	<algorithm>

# define	DEFAULT_MIME_FILE		"./srcs/http/Message/default_mimes.txt"
# define	MIME_FILE_DELIM			'\n'
# define	MIME_CGI				"cgi/response"

namespace	http {
	std::string	getSuffix(const std::string &fileREF);
	std::string	getFileResource(const std::string &uri);
	typedef std::pair< std::vector< std::string >, std::string >	t_mime_pair;
	typedef	std::vector< t_mime_pair >								t_mime_map;
	extern const t_mime_map	mimeTypes;
	class IsMimeTypeUnary {
		private:
			std::string	_mimeType;
		protected:
		public:
			IsMimeTypeUnary(const std::string &mimeType);
			bool	operator()(const t_mime_pair &mimePairREF);
	};
	std::string	checkMimeType(const std::string &uriREF);
	t_mime_map	loadMimeFile(void);
}

#endif