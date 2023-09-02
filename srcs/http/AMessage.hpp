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

class	AMessage {
	protected:
		std::string										_startLine;
		std::list<std::pair<std::string, std::string>>	_headers;
		std::string										_messageBody;
	public:
		AMessage(void);
		AMessage(AMessage &aMessageREF);
		AMessage(std::string startLine, std::list<std::pair<std::string,
				std::string>> headers, std::string messageBody);
		AMessage(std::string rawMessage);
		std::string	getStartLine(void);
		std::list<std::pair<std::string, std::string>>	getHeaders(void);
		virtual		~AMessage(void);
		AMessage	&operator=(AMessage &aMessageREF);
		std::string	getMessageBody(void);
		std::string	getRawMessage(void);
};

#endif