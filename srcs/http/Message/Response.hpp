/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:36:49 by kalmheir          #+#    #+#             */
/*   Updated: 2023/12/13 18:25:38 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include	<iostream>
# include	<string>
# include	<exception>
# include	"ServerConfig.hpp"
# include	"AMessage.hpp"
# include	"Request.hpp"
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<unistd.h>

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
		protected:
			std::string		_httpVersion;
			unsigned short	_httpStatusCode;
			bool			_ready;
		public:
			Response(void);
			Response(const Response &responseREF);
			virtual ~Response(void);
			Response	&operator=(const Response &responseREF);
			Response(int status);
			Response(int status, const std::string &responseBody);
			Response(std::string httpRaw);

			std::string		getHttpVersion(void) const;
			unsigned short	getHttpStatusCode(void) const;
			std::string		getHttpStatusString(unsigned short statusCode) const;
			Response		buildResponse(const Request &requestREF, const ServerConfig &servConfREF);
			bool			validate(void) const;
			bool			responseReady() const;
			void			setResponseStatus(bool status);

		private:
	};
	class ErrorPageResponse : public Response
	{
	private:
	protected:
	public:
		ErrorPageResponse(void);
		virtual ~ErrorPageResponse(void);
		ErrorPageResponse(const ErrorPageResponse &eprREF);
		ErrorPageResponse &operator=(const ErrorPageResponse &eprREF);
		ErrorPageResponse(const int &status, const ServerConfig &servConfREF, const ServerConfig::Location *locPTR);
	};

	class RedirectResponse : public Response
	{
	private:
	protected:
	public:
		RedirectResponse(void);
		virtual ~RedirectResponse(void);
		RedirectResponse(const RedirectResponse &rrREF);
		RedirectResponse &operator=(const RedirectResponse &rrREF);
		RedirectResponse(const ServerConfig &servConfREF, const ServerConfig::Location *locPTR);
	};

	class ResponseException : public std::exception
	{
	private:
		std::string _errorMsg;

	public:
		virtual ~ResponseException() throw(){};
		ResponseException(const std::string &errorMsg)
		{
			_errorMsg = errorMsg;
		};
		virtual const char *what() const throw()
		{
			return (_errorMsg.c_str());
		};
	};
};



#endif