/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalmheir <kalmheir@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 11:19:21 by kalmheir          #+#    #+#             */
/*   Updated: 2023/09/02 13:04:55 by kalmheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include "Http_namespace.hpp"

namespace http {
    class Header {
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
    };
}

#endif