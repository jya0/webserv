/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:19:21 by kalmheir          #+#    #+#             */
/*   Updated: 2023/11/30 18:27:45 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>

# define	CR_LF					"\r\n"

# define	HEADER_KEY_AUTH			"Authorization"
# define	HEADER_KEY_CONTENT_TYPE	"Content-Type"

namespace http {
    class Header {
		private:
        protected:
            std::string _name;
            std::string _value;
        public:
            Header(void);
            Header(std::string name, std::string value);
            Header(Header const & src);
            virtual ~Header();
            Header & operator=(Header const & src);
            std::string toString(void) const;
            std::string getName(void) const;
            std::string getValue(void) const;

			class	IsHeaderKeyUnary;
    };
}
class	http::Header::IsHeaderKeyUnary {
	private:
		std::string	_headerKeyToFind;
	protected:
	public:
		IsHeaderKeyUnary(const std::string &headerKeyToFind): _headerKeyToFind(headerKeyToFind) {};
		bool	operator()(const Header &headerREF)
		{
			return (headerREF.getName() == _headerKeyToFind);
		};
};

#endif