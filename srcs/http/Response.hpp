/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.class.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:44:08 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 18:22:40 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include	<iostream>
# include	<string>
# include	"AMessage.hpp"

class	Response: private AMessage
{
	private:
		std::string		_httpVersion;
		unsigned short	_httpStatusCode;
	protected:
	public:
		Response(void);
		Response(int status);
		Response(std::string httpRaw);
		Response(Response &responseREF);
		~Response(void);
		Response	&operator=(Response &responseREF);
		std::string	getHttpVersion(void);
		unsigned short	getHttpStatusCode(void);
};


#endif