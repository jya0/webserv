/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/12/03 15:54:11 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include	"ADirective.hpp"
# include	"DirectiveBlock.hpp"
# include	"DirectiveSimple.hpp"
# include	"ServerConfig.hpp"

# define	SPACE_CHARSET	" \n\t\v\f\r"

class HTTPServerParser {
private:
    HTTPServerParser();
public:
    static std::vector< ServerConfig >	parseConfigFile(std::string filename);
	static const std::vector< std::string >	dveNames;
    ~HTTPServerParser();

	class ParseErrorException;

};

class HTTPServerParser::ParseErrorException: public std::exception {
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