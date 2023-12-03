/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:36:49 by kalmheir          #+#    #+#             */
/*   Updated: 2023/11/30 21:43:50 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include	<iostream>
# include	<string>
# include	<exception>
# include	"ServerConfig.hpp"
# include   "AMessage.hpp"
# include	"Request.hpp"

class	ServerConfig;

namespace http {
	/**
	 * @brief This class describes an HTTP Response.
	 *
	 * @field _httpVersion			The HTTP version of the response.
	 * @field _httpStatusCode		The HTTP status code of the response.
	 * @method getHttpVersion		Returns the HTTP version of the response.
	 * @method getHttpStatusCode	Returns the HTTP status code of the response.
	 * @method validate				Validates the headers of the response.
	 */
	class	Response: public AMessage
	{
		private:
			std::string		_httpVersion;
			unsigned short	_httpStatusCode;
			bool			_ready;
		protected:
		public:
			Response(void);
			Response(const Response &responseREF);
			~Response(void);
			Response	&operator=(const Response &responseREF);
			Response(int status);
			Response(int status, const std::string &responseBody);
			Response(std::string httpRaw);

			std::string		getHttpVersion(void) const;
			unsigned short	getHttpStatusCode(void) const;
			std::string		getHttpStatusString(unsigned short statusCode) const;
			Response		buildResponse(const Request &requestREFREF, const ServerConfig &servConfREF);
			bool			validate(void) const;
			bool			responseReady() const;
			void			setResponseStatus(bool status);

			class	ResponseException;
	};
};

class	http::Response::ResponseException: public std::exception {
		private:
			std::string	_errorMsg;
    	public:
			virtual ~ResponseException() throw () {};
			ResponseException(const std::string &errorMsg) {
				_errorMsg = errorMsg;
			};
			virtual const char *what() const throw()
			{
				return (_errorMsg.c_str());
			};
};

#endif