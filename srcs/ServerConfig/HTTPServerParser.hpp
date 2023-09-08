/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/09/08 10:50:18 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include   "WebServer.hpp"

class HTTPServerParser {
private:
    HTTPServerParser();

public:
    // static std::vector<WebServer>   parseConfigFile(std::string filename);
    static void parseConfigFile1(std::string filename);
    ~HTTPServerParser();

    class ParseError : public std::exception {
    public:
        virtual const char *what() const throw();
    };
};


#endif