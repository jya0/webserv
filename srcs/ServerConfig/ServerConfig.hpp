/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:55:49 by jyao              #+#    #+#             */
/*   Updated: 2023/12/09 03:00:53 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_CONFIG_HPP
# define	SERVER_CONFIG_HPP

# include	<iostream>
# include	<utility>
# include	<limits>
# include	<vector>
# include	"DirectiveBlock.hpp"
# include	"Response.hpp"

# define	DEFAULT_AUTO_INDEX				false
# define	DEFAULT_CGI_BIN					"CGI/"
# define	DEFAULT_CMB_SIZE				42424242ul
# define	DEFAULT_INDEX					"index.html"
# define	DEFAULT_LISTEN_IP				"127.0.0.1"
# define	DEFAULT_LISTEN_PORT				8000
# define	DEFAULT_RETURN_CODE				-1	//not defined by NGINX
# define	DEFAULT_RETURN_URI				""	//not defined by NGINX
# define	DEFAULT_ROOT					"./"	//will be cwd if not specified
# define	DEFAULT_SERVER_NAMES			""
# define	DEFAULT_LIMIT_EXCEPT_METHODS	0xffffff
# define	DEFAULT_LOCATION_URI			"" //not defined by NGINX

typedef std::pair< std::vector< int >, std::string >	t_errorPage;

class	ServerConfig {
	public:
		class	Location;
	private:
		std::pair< std::string, int >	_listen;
		std::vector< std::string >		_serverNames;
		std::vector< Location >			_locations;
	protected:
		bool							_autoIndex;
		std::size_t						_sizeCMB;
		std::vector< t_errorPage >		_errorPages;
		std::vector< std::string >		_index;
		std::pair< int, std::string >	_return;
		std::string						_root;
	public:
		ServerConfig(void);
		ServerConfig(DirectiveBlock *serverDveBlock);
		virtual ~ServerConfig(void);
		ServerConfig(const ServerConfig &servConfREF);
		ServerConfig &operator=(const ServerConfig &servConfREF);

		const std::vector< std::string >		&getIndex(void) const;
		const std::pair< std::string, int >		&getListen(void) const;
		const std::vector< std::string >		&getServerNames(void) const;
		const std::vector< Location >			&getLocations(void) const;
		std::vector< Location >::const_iterator	getLocation(const std::string &uriREF) const;
		const bool								&getAutoIndex(void) const;
		const std::size_t						&getSizeCMB(void) const;
		const std::vector< t_errorPage >		&getErrorPages(void) const;
		std::string								getErrorPage(const int &statusCode) const;
		const std::pair< int, std::string >		&getReturn(void) const;
		const std::string						&getRoot(void) const;
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
				LimitExcept(const std::vector< std::string > &methodStrs);
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

		class	IsLocationUnary;
};

class	ServerConfig::Location::IsLocationUnary {
	private:
		std::string	_uri;
	protected:
	public:
		IsLocationUnary(const std::string &uri): _uri(uri) {};
		bool	operator()(const Location &locationREF)
		{
			return (_uri.compare(0, locationREF.locationUri.size(), locationREF.locationUri) == 0);
		};
};

#endif