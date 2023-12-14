/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:01:55 by jyao              #+#    #+#             */
/*   Updated: 2023/12/14 04:51:22 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		CGI_HANDLER_HPP
# define	CGI_HANDLER_HPP

# include	<map>
# include	<string>
# include	<exception>
# include	<unistd.h>
# include	<cstdio>
# include	"Request.hpp"
# include	"ServerConfig.hpp"
# include	"PollManager.hpp"
#include	<stdio.h>
#include	<ctime>
#include	<iostream>

# define	GATEWAY_INTERFACE	"CGI/1.1"
# define	SERVER_PROTOCOL		"HTTP/1.1"
# define	SERVER_SOFTWARE		"42-webserv-team-kry/0.1"
# define	QUERY_DELIM			'?'

# define	READ_BUF_SIZE		424242

namespace	http {
	class	CGIhandler {
		public:
			class	CGIexception;
		private:
			std::map< std::string, std::string >	_cgiEnv;
			http::Request							_cgiRequest;
			pid_t									_childPid;
			FILE									*_inFile;
			int										_inFileFd;
			FILE									*_outFile;
			int										_outFileFd;
			int										_cinSave;
			int										_coutSave;
			std::clock_t 							_startTime;
			int										_clientSocket;
			int										_serverSocket;
		protected:
		public:
			static PollManager						*sockets;
			static void setPollManager(PollManager &socketsREF);
			CGIhandler(void);
			CGIhandler(const CGIhandler &cgiREF);
			~CGIhandler(void);
			CGIhandler	&operator=(const  CGIhandler &cgiREF);
			CGIhandler(const http::Request &requestREF, const ServerConfig::Location *locationPTR);

			const std::map< std::string, std::string >	&getCgiEnv(void) const;
			const http::Request							&getCgiRequest(void) const;

			const std::clock_t	&getStartTime() const;
			const pid_t			&getChildPid() const;
			const int			&getCinSave() const;
			const int			&getCoutSave() const;
			const int			&getInFileFd() const;
			const int			&getOutFileFd() const;
			const FILE 			*getInFile() const;
			const FILE 			*getOutFile() const;
			const int			&getClientSocket() const;
			const int			&getServerSocket() const;
			void setServerSocket(int serverSocket);
			void setClientSocket(int clientSocket);
			void executeCGI(const std::string &scriptName);
			void createTmpFiles();
			void closeChildFds();
			void closeParentFds() const;
			void deleteEnvArr(char *const *envArr);
			void runCGI(const std::string &scriptName);

			char **mapToArr(const std::map<std::string, std::string> &cgiEnvREF);
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