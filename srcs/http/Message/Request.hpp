/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:37:54 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/15 05:24:01 by rriyas           ###   ########.fr       */
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

			void				parseChuncked(void);
			void				parseRequest(void);
			const e_httpMethod	&getHttpMethodEnum(void) const;
			const std::string	&getHttpVersion(void) const;
			std::string			getHttpMethod(void) const;
			const std::string	&getUri(void) const;
			bool				validate(void) const;
			void				appendRawData(const std::string &_data);
			bool				requestReady() const;
			void				setRequestStatus(bool status);
			FILE				*getRawData() const;
			bool				recievedEOF();
			bool				recievedLastChunk();
			bool				recievedLastByte();
	};
};

#endif