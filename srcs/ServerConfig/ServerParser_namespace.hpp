/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser_namespace.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/12/05 16:13:16 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include	"ADirective.hpp"
# include	"DirectiveBlock.hpp"
# include	"DirectiveSimple.hpp"
# include	"ServerConfig.hpp"
# include	"ToString.tpp"

# define	SPACE_CHARSET	" \n\t\v\f\r"

namespace	ServerParser {
	enum	e_errors {
		ERROR_COLON			= 1,
		ERROR_DVE_NAME,
		ERROR_DVE_VALUE,
		ERROR_DVE_TYPE,
		ERROR_BAD_CONTEXT
	};
    std::vector< ServerConfig >				parseConfigFile(const std::string &filename);
	std::string								appendSlashes(std::string uri);
	extern const std::vector< std::string >	dveNames;
	std::vector< std::string >				tokenize(const std::string &lineREF);
	std::pair< std::string, std::string >	splitByTwo(const std::string &strREF, const char &delimREF);

	class ParseErrorException;
};

class ServerParser::ParseErrorException: public std::exception {
		private:
			std::string	_errorMsg;
    	public:
			virtual ~ParseErrorException() throw () {};
			ParseErrorException(const std::string &errorMsg) {
				_errorMsg = errorMsg;
			};
			virtual const char *what() const throw()
			{
				return (_errorMsg.c_str());
			};
};

#endif