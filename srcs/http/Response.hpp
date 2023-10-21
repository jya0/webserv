/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:36:49 by kalmheir          #+#    #+#             */
/*   Updated: 2023/09/16 16:38:09 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include	<iostream>
# include	<string>
# include   "Http_namespace.hpp"

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
		protected:
		public:
			Response(void);
			Response(int status);
			Response(std::string httpRaw);
			Response(const Response &responseREF);
			~Response(void);
			Response	&operator=(const Response &responseREF);
			std::string	getHttpVersion(void) const;
			unsigned short	getHttpStatusCode(void) const;
			bool validate(void) const;
			std::string getHttpStatusString(unsigned short statusCode) const;
	};
};

#endif