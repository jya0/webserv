/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 21:00:20 by jyao              #+#    #+#             */
/*   Updated: 2023/12/15 00:11:08 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTP_AMESSAGE_HPP
# define HTTP_AMESSAGE_HPP

# include	<iostream>
# include	<string>
# include	<list>
# include	<utility>
# include	<cstdio>
# include	"Header.hpp"

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
		private:
		protected:
			std::string			_startLine;
			std::list<Header>	_headers;
			FILE				*_raw;
			fpos_t				_messageBody;
			size_t				_messageBodySize;
		public:
			AMessage(void);
			AMessage(const AMessage &aMessageREF);
			AMessage(std::string startLine, std::list<Header> headers,
				std::string messageBody, size_t messageBodySize);
			AMessage(std::string rawMessage);
			std::string	getStartLine(void);
			std::list<Header>	getHeaders(void) const;
			std::string			getHeaderValue(const std::string &headerKey) const;
			virtual		~AMessage(void);
			AMessage	&operator=(const AMessage &aMessageREF);
			std::string	getMessageBody(void) const;
			std::string	getRawMessage(void) const;
			void addHeader(Header header);
			virtual bool validate(void) const = 0;
			void setMessageBody(std::string messageBody);
			void setStartLine(std::string startLine);
			void parseMessageBody();

	};
}

#endif