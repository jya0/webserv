/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messge.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 17:44:03 by jyao              #+#    #+#             */
/*   Updated: 2023/08/13 18:10:33 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_AMESSAGE_HPP
# define HTTP_AMESSAGE_HPP

# include	<iostream>
# include	<string>
# include	<list>
# include	<utility>
# include	"Http_namespace.hpp"

namespace http {
	/**
	 * @brief 	This class describes a TCP/HTTP message. 
	 * @field	_startLine		The first line of the message.
	 * @field	_headers		The list of headers of the message.
	 * @field	_messageBody	The body of the message.
	 * @method	getStartLine	Returns the start line of the message.
	 * @method	getHeaders		Returns the headers of the message.
	 * @method	getMessageBody	Returns the body of the message.
	 * @method	getRawMessage	Returns the raw message.
	 * @method	validate		Validates the message.
	 */
	class	AMessage {
		protected:
			std::string			_startLine;
			std::list<Header>	_headers;
			std::string			_messageBody;
		public:
			AMessage(void);
			AMessage(AMessage &aMessageREF);
			AMessage(std::string startLine, std::list<Header> headers,
				std::string messageBody);
			AMessage(std::string rawMessage);
			std::string	getStartLine(void);
			std::list<Header>	getHeaders(void);
			virtual		~AMessage(void);
			AMessage	&operator=(AMessage &aMessageREF);
			std::string	getMessageBody(void);
			std::string	getRawMessage(void);
			virtual bool validate(void) = 0;
	};
}

#endif