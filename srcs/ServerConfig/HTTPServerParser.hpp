/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 15:21:32 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include	"ADirective.hpp"
# include	"DirectiveBlock.hpp"
# include	"DirectiveSimple.hpp"

/**
 * @brief an abstract class which contains non-member methods 
 * used to parse the config file.
 * 
 * @field dveNames is a non-member attribute which contains a vector of all the
 * directive names, used to get the _dveType of directives later in the
 * ADirective::parserDirective(void) method
 * 
 * @method parseConfigFile(std::string filename) used to return a vector of 
 * server block directives
 * 
 */
class HTTPServerParser {
private:
    HTTPServerParser();
public:
	static const std::vector< std::string >	dveNames;
    static std::vector< DirectiveBlock * >	parseConfigFile(std::string filename);
    ~HTTPServerParser();

    class ParseError : public std::exception {
    public:
        virtual const char *what() const throw();
    };
};

#endif