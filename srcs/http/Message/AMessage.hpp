/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 21:00:20 by jyao              #+#    #+#             */
/*   Updated: 2023/12/16 04:24:01 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_AMESSAGE_HPP
#define HTTP_AMESSAGE_HPP

#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <cstdio>
#include "Header.hpp"

#define MSG_BODY_BUFFER 424242

namespace http {
	std::string	fileToString(FILE *file);
	size_t 		getFileSize(FILE *file);
	FILE		*duplicateFile(const FILE *input);
	void		filecpy(FILE *src, FILE *dst);
	typedef std::pair< std::string, FILE * >	t_raw_message;

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
	class AMessage
	{
	private:
	protected:
		std::string _startLine;
		std::list<Header> _headers;
		std::string _httpVersion;
		bool _ready;
		FILE *_messageBody;
		size_t	_bodySize;

	public:
		AMessage(void);
		AMessage(const AMessage &aMessageREF);
		AMessage(std::string messageHeader);
		virtual ~AMessage(void);
		AMessage &operator=(const AMessage &aMessageREF);

		std::string			getStartLine(void);
		std::list< Header >	getHeaders(void) const;
		std::string			getHeaderValue(const std::string &headerKey) const;
		FILE				*getMessageBody(void) const;
		std::string			getMessageBodyStr(void) const;
		void 				addHeader(Header header);
		void				setMessageBody(const std::string &msgBodyREF);
		void				setMessageBody(FILE *msgBody);
		void 				setStartLine(std::string startLine);
		void				loadFileToMessageBody(const std::string &filePathREF);
		std::string			getStartAndHeader(void) const;
		t_raw_message		getRawMessage(void) const;
	};
}

#endif