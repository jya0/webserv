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

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include	<iostream>
# include	<string>
# include	"Message.hpp"

class	Request: private Message
{
	private:
		std::string	_httpMethod;
		std::string	_uri;
	protected:
	public:
		Request(void);
		Request(Request const &RequestREF);
		Request(std::string httpRaw);
		~Request(void);
		Request	&operator=(Request const &RequestREF);
};

#endif