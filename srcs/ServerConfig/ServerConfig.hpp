/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:55:49 by jyao              #+#    #+#             */
/*   Updated: 2023/10/22 16:54:31 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_CONFIG_HPP
# define	SERVER_CONFIG_HPP

# include	<iostream>
# include	<vector>
# include	"Request.hpp"

class	ServerConfig {
	private:
		std::string					_cgiBin;
		std::string					_index;
		std::string					_listen;
		std::vector<std::string>	_serverNames;
		class	Location;
		std::vector<Location>		_locations;
	protected:
		bool						_autoIndex;
		std::size_t					_sizeCMB;
		std::string					_errorPage;
		class	Return {
			private:
			protected:
			public:
				int			statusCode;
				std::string	uri;

				Return(void);
				~Return(void);
				Return(const Return &returnREF);
				Return &operator=(const Return &returnREF);
		};
		Return						_return;
		std::string					_root;
	public:
		ServerConfig(void);
		virtual ~ServerConfig(void);
		ServerConfig(const ServerConfig &serverConfigREF);
		ServerConfig &operator=(const ServerConfig &serverConfigREF);

		std::string	getCgiBin(void) const;
		std::string	getIndex(void) const;
		std::string	getListen(void) const;
		std::vector<std::string>	getServerNames(void) const;
		std::vector<Location>		getLocations(void) const;
		bool	getAutoIndex(void) const;
		std::size_t	getSizeCMB(void) const;
		std::string	getErrorPage(void) const;
		Return	getReturn(void) const;
		std::string	getRoot(void) const;
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
				LimitExcept(const LimitExcept &limitExceptREF);
				LimitExcept &operator=(const LimitExcept &limitExceptREF);
		};
	protected:
	public:
		LimitExcept	limitExcept;

		Location(void);
		~Location(void);
		Location(const Location &locationREF);
		Location &operator=(const Location &locationREF);
};

#endif