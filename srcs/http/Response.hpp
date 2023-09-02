/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:36:49 by kalmheir          #+#    #+#             */
/*   Updated: 2023/09/02 14:36:56 by kalmheir         ###   ########.fr       */
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
	 * @field _httpVersion		The HTTP version of the response.
	 * @field _httpStatusCode	The HTTP status code of the response.
	 * @method getHttpVersion	Returns the HTTP version of the response.
	 * @method getHttpStatusCode	Returns the HTTP status code of the response.
	 * @method validate			Validates the headers of the response.
	 */
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
			bool validate(void);
	};
};

#endif