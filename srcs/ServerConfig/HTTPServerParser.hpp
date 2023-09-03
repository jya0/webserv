/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:46:11 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 10:17:52 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_PARSER_HPP
# define HTTP_SERVER_PARSER_HPP

# include   "../../inc/WebServer.hpp"

class HTTPServerParser {
private:
    HTTPServerParser();

public:
    static std::vector<WebServer> &parseConfigFile(std::string filename);
    ~HTTPServerParser();

    class ParseError : public std::exception {
    public:
        virtual const char *what() const throw();
    };
};


#endif