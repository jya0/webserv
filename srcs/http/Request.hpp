/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:44:06 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 18:22:40 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include	<iostream>
# include	<string>
# include	"AMessage.hpp"

class	Request: private AMessage {
	private:
		std::string	_httpMethod;
		std::string _httpVersion;
		std::string	_uri;
	protected:
	public:
		Request(void);
		Request(Request const &RequestREF);
		Request(std::string httpRaw);
		~Request(void);
		Request	&operator=(Request const &RequestREF);
		std::string	getHttpMethod(void);
		std::string	getHttpVersion(void);
		std::string	getUri(void);
};

#endif