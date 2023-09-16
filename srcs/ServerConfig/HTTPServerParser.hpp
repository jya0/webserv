/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/09/15 12:13:34 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include	"ADirective.hpp"
# include	"DirectiveBlock.hpp"
# include	"DirectiveSimple.hpp"

# define	SPACE_CHARSET	" \n\t\v\f\r"

class HTTPServerParser {
private:
    HTTPServerParser();
public:
    static std::vector< DirectiveBlock * >	parseConfigFile(std::string filename);
	static const std::vector< std::string >	dveNames;
    ~HTTPServerParser();

    class ParseError : public std::exception {
    public:
        virtual const char *what() const throw();
    };
};

#endif