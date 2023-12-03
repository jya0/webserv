/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:01:55 by jyao              #+#    #+#             */
/*   Updated: 2023/11/30 12:11:33 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		CGI_HANDLER_HPP
# define	CGI_HANDLER_HPP

# include	<map>
# include	<string>
# include	<exception>
# include	"Request.hpp"
# include	"ServerConfig.hpp"

# define	GATEWAY_INTERFACE	"CGI/1.1"
# define	SERVER_PROTOCOL		"HTTP/1.1"
# define	SERVER_SOFTWARE		"42-webserv-team-kry/0.1"

# define	READ_BUF_SIZE		424242

namespace	http {
	class	CGIhandler {
		public:
			class	CGIexception;
		private:
			std::map< std::string, std::string >	_cgiEnv;
			http::Request							_cgiRequest;
		protected:
		public:
			CGIhandler(void);
			CGIhandler(const CGIhandler &cgiREF);
			~CGIhandler(void);
			CGIhandler	&operator=(const CGIhandler &cgiREF);
			CGIhandler(const http::Request &requestREF, const ServerConfig::Location &locationREF);

			const std::map< std::string, std::string >	&getCgiEnv(void) const;
			const http::Request							&getCgiRequest(void) const;

			std::string	executeCGI(const std::string &scriptName) throw (std::exception);
	};
}

class	http::CGIhandler::CGIexception: public std::exception {
		private:
			std::string	_errorMsg;
    	public:
			virtual ~CGIexception() throw () {};
			CGIexception(const std::string &errorMsg) {
				_errorMsg = errorMsg;
			};
			virtual const char *what() const throw()
			{
				return (_errorMsg.c_str());
			};
};

#endif