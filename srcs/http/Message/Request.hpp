/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:37:54 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/16 04:25:15 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include	<iostream>
# include	<string>
# include	<cstdio>
# include	"AMessage.hpp"

# define	GET_METHOD		"GET"
# define	POST_METHOD		"POST"
# define	HEAD_METHOD		"HEAD"
# define	PUT_METHOD		"PUT"
# define	DELETE_METHOD	"DELETE"

# define	HEX_STR_BUFFER	16
# define	RFIND_RANGE		42
# define	CR				'\r'

/**
 * @brief An enum for the HTTP methods supported by the server.
 */
enum e_httpMethod {
	NO_METHOD = 0,
	GET = 1,
	POST = GET << 1,
	HEAD = POST << 1,
	PUT = HEAD << 1,
	DELETE = PUT << 1
};

namespace http {
	FILE	*duplicateFile(const FILE *input);
	void	printFile(FILE *file);
    bool    rfind(FILE *haystack, std::string needle);
	/**
	 * @brief This class describes an HTTP Request.
	 * @field _httpMethod		The HTTP method of the request.
	 * @field _httpVersion		The HTTP version of the request.
	 * @field _uri				The URI of the request.
	 * @method getHttpMethod	Returns the HTTP method of the request.
	 * @method getHttpVersion	Returns the HTTP version of the request.
	 * @method getUri			Returns the URI of the request.
	 * @method validate			Validates the headers of the request.
	 * @method methodName		Returns the name of an HTTP method enum.
	 * @method methodEnum		Returns the enum of an HTTP method name.
	 */
	class	Request: public AMessage {
		private:
			e_httpMethod	_httpMethod;
			std::string		_uri;
		protected:
		public:
			Request(void);
			Request(Request const &RequestREF);
			Request	&operator=(Request const &RequestREF);
			Request(const std::string &messageHeader);
			~Request(void);

			static std::string	methodName(e_httpMethod method);
			static e_httpMethod	methodEnum(const std::string method);

			void				parseChunked(void);
			void				parseRequest(void);
			const e_httpMethod	&getHttpMethodEnum(void) const;
			const std::string	&getHttpVersion(void) const;
			std::string			getHttpMethod(void) const;
			const std::string	&getUri(void) const;
			bool				validate(void) const;
			void				appendRawData(const std::string &_data);
			bool				requestReady() const;
			void				setRequestStatus(bool status);
			bool				recievedEOF();
			bool				recievedLastByte();
	};
};

#endif