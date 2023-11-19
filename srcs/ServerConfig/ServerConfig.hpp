/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:55:49 by jyao              #+#    #+#             */
/*   Updated: 2023/11/19 13:50:46 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_CONFIG_HPP
# define	SERVER_CONFIG_HPP

# include	<iostream>
# include	<vector>
# include	<limits>
# include	<utility>
# include	"DirectiveBlock.hpp"
# include	"Request.hpp"

# define	DEFAULT_AUTO_INDEX				false
# define	DEFAULT_CGI_BIN					"CGI/"
# define	DEFAULT_CMB_SIZE				42424242ul
# define	DEFAULT_ERROR_PAGE				"error_page"
# define	DEFAULT_INDEX					"index.html"
# define	DEFAULT_LISTEN_IP				"127.0.0.1"
# define	DEFAULT_LISTEN_PORT				8000
# define	DEFAULT_RETURN_CODE				-1	//not defined by NGINX
# define	DEFAULT_RETURN_URI				""	//not defined by NGINX
# define	DEFAULT_ROOT					""	//current working directory
# define	DEFAULT_SERVER_NAMES			""
# define	DEFAULT_LIMIT_EXCEPT_METHODS	0xffffff
# define	DEFAULT_LOCATION_URI			"" //not defined by NGINX

class	ServerConfig {
	private:
		std::string							_cgiBin;
		std::pair< std::string, int >		_listen;
		std::vector< std::string >			_serverNames;
		class	Location;
		std::vector< Location >				_locations;
	protected:
		bool								_autoIndex;
		std::size_t							_sizeCMB;
		std::string							_errorPage;
		std::vector< std::string >			_index;
		std::pair< int, std::string >		_return;
		std::string							_root;
	public:
		ServerConfig(void);
		ServerConfig(DirectiveBlock *serverDveBlock);
		virtual ~ServerConfig(void);
		ServerConfig(const ServerConfig &serverConfigREF);
		ServerConfig &operator=(const ServerConfig &serverConfigREF);

		std::string						getCgiBin(void) const;
		std::vector< std::string >		getIndex(void) const;
		std::pair< std::string, int >	getListen(void) const;
		std::vector< std::string >		getServerNames(void) const;
		std::vector< Location >			getLocations(void) const;
		bool							getAutoIndex(void) const;
		std::size_t						getSizeCMB(void) const;
		std::string						getErrorPage(void) const;
		std::pair< int, std::string >	getReturn(void) const;
		std::string						getRoot(void) const;
};

class	ServerConfig::Location: public ServerConfig {
	private:
		class	LimitExcept {
			private:
			protected:
			public:
				int	acceptedMethods;

				LimitExcept(void);
				~LimitExcept(void);
				LimitExcept(ADirective *locationDveBlock);
				LimitExcept(const LimitExcept &limitExceptREF);
				LimitExcept &operator=(const LimitExcept &limitExceptREF);
		};
	protected:
	public:
		std::string	locationUri;
		LimitExcept	limitExcept;

		Location(void);
		Location(ADirective *locationDveBlock);
		~Location(void);
		Location(const Location &locationREF);
		Location &operator=(const Location &locationREF);
};

#endif