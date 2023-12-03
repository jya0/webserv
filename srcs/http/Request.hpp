/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 13:37:54 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/03 15:13:01 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include	<iostream>
# include	<string>
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
	GET = 1,
	POST = GET << 1,
	HEAD = POST << 1,
	PUT = HEAD << 1,
	DELETE = PUT << 1
};

namespace http {
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
			std::string		_httpVersion;
			std::string		_uri;
			std::string		_raw;
			bool			_ready;
		protected:
		public:
			Request(void);
			Request(Request const &RequestREF);
			Request	&operator=(Request const &RequestREF);
			~Request(void);
			Request(std::string httpRaw);

			static std::string methodName(e_httpMethod method);
			static e_httpMethod methodEnum(const std::string method);

			const e_httpMethod	&getHttpMethodEnum(void) const;
			const std::string	&getHttpVersion(void) const;
			std::string	getHttpMethod(void) const;
			const std::string	&getUri(void) const;
			bool validate(void) const;
			void appendRawData(std::string _data);
			bool requestReady() const;
			void setRequestStatus(bool status);
			std::string getRawData();
	};
};

#endif