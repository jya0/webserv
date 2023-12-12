/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:55:49 by jyao              #+#    #+#             */
/*   Updated: 2023/12/12 22:46:50 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_CONFIG_HPP
# define	SERVER_CONFIG_HPP

# include	<iostream>
# include	<utility>
# include	<limits>
# include	<vector>
# include	"DirectiveBlock.hpp"
# include	"MimeTypes.hpp"

# define	DEFAULT_AUTO_INDEX				false
# define	DEFAULT_CMB_SIZE				-1
# define	DEFAULT_INDEX					""
# define	DEFAULT_LISTEN_IP				"127.0.0.1"
# define	DEFAULT_LISTEN_PORT				80
# define	DEFAULT_RETURN_CODE				301	//not defined by NGINX
# define	DEFAULT_RETURN_URI				""	//not defined by NGINX
# define	DEFAULT_ROOT					""	//will be cwd if not specified
# define	DEFAULT_SERVER_NAMES			""
# define	DEFAULT_LIMIT_EXCEPT_METHODS	0xffffff
# define	DEFAULT_LOCATION_URI			"" //not defined by NGINX

typedef std::pair< std::vector< int >, std::string >	t_errorPage;

class	ServerConfig {
	public:
		class	Return {
			private:
			protected:
			public:
				bool		isInit;
				int			code;
				std::string	uri;

				Return(void);
				~Return(void);
				Return(const Return &returnREF);
				Return	&operator=(const Return &returnREF);
		};
		class	Location;
	private:
		std::pair< std::string, int >	_listen;
		std::vector< std::string >		_serverNames;
		std::vector< Location >			_locations;
	protected:
		bool							_autoIndex;
		ssize_t							_sizeCMB;
		std::vector<std::pair<std::vector<int>, std::string> > _errorPages;
		std::vector< std::string >		_index;
		Return							_return;
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
		const Location							*getLocation(const std::string &uriREF) const;
		const bool								&getAutoIndex(void) const;
		const ssize_t							&getSizeCMB(void) const;
		const std::vector< std::pair< std::vector< int >, std::string > >		&getErrorPages(void) const;
		std::string						getErrorPage(const int &statusCode) const;
		const Return							&getReturn(void) const;
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
			std::string	file;

			if (_uri.empty())
				return (false);
			file = http::getFileResource(_uri);
			if (http::checkMimeType(file) == MIME_CGI)
				return (('.' + http::getSuffix(file)) == locationREF.locationUri);
			return (_uri.compare(0, locationREF.locationUri.size(), locationREF.locationUri) == 0);
		};
};

#endif