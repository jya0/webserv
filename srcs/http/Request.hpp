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
# include	"Http_namespace.hpp"

enum e_httpMethod {
	GET,
	POST,
	HEAD,
	PUT,
	DELETE
};

namespace http {
	class	Request: private AMessage {
		private:
			e_httpMethod	_httpMethod;
			std::string		_httpVersion;
			std::string		_uri;
		protected:
		public:
			static std::string methodName(e_httpMethod method);
			static e_httpMethod methodEnum(std::string method);
			Request(void);
			Request(Request const &RequestREF);
			Request(std::string httpRaw);
			~Request(void);
			Request	&operator=(Request const &RequestREF);
			std::string	getHttpMethod(void);
			std::string	getHttpVersion(void);
			std::string	getUri(void);
			bool validate(void);
	};
};

#endif