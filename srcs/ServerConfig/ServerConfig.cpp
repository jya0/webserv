/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:57:39 by jyao              #+#    #+#             */
/*   Updated: 2023/10/22 16:39:25 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"ServerConfig.hpp"

/* class ServerConfig */
ServerConfig::ServerConfig(void) {};

ServerConfig::~ServerConfig(void) {};

ServerConfig::ServerConfig(const ServerConfig &serverConfigREF)
{
	this->ServerConfig::operator=(serverConfigREF);
};

ServerConfig &ServerConfig::operator=(const ServerConfig &serverConfigREF)
{
	if (this != &serverConfigREF)
	{
		_cgiBin = serverConfigREF.getCgiBin();
		_index = serverConfigREF.getIndex();
		_listen = serverConfigREF.getListen();
		_serverNames = serverConfigREF.getServerNames();
		_locations = serverConfigREF.getLocations();
		_autoIndex = serverConfigREF.getAutoIndex();
		_sizeCMB = serverConfigREF.getSizeCMB();
		_errorPage = serverConfigREF.getErrorPage();
		_return = serverConfigREF.getReturn();
		_root = serverConfigREF.getRoot();
	}
	return (*this);
};

std::string	ServerConfig::getCgiBin(void) const {
	return (_cgiBin);
};

std::string	ServerConfig::getIndex(void) const {
	return (_index);
};

std::string	ServerConfig::getListen(void) const {
	return (_listen);
};

std::vector<std::string>	ServerConfig::getServerNames(void) const {
	return (_serverNames);
};

std::vector<ServerConfig::Location>		ServerConfig::getLocations(void) const {
	return (_locations);
};

bool	ServerConfig::getAutoIndex(void) const {
	return (_autoIndex);
};

std::size_t	ServerConfig::getSizeCMB(void) const {
	return (_sizeCMB);
};

std::string	ServerConfig::getErrorPage(void) const {
	return (_errorPage);
};

ServerConfig::Return	ServerConfig::getReturn(void) const {
	return (_return);
};

std::string	ServerConfig::getRoot(void) const {
	return (_root);
};

/* class ServerConfig::Return */
ServerConfig::Return::Return(void) {};

ServerConfig::Return::~Return(void) {};

ServerConfig::Return::Return(const Return &returnREF)
{
	this->ServerConfig::Return::operator=(returnREF);
};

ServerConfig::Return &ServerConfig::Return::operator=(const Return &returnREF)
{
	if (this != &returnREF)
	{
		statusCode = returnREF.statusCode;
		uri = returnREF.uri;
	}
	return (*this);
};

/* class ServerConfig::Location */
ServerConfig::Location::Location(void): ServerConfig() {};

ServerConfig::Location::~Location(void) {};

ServerConfig::Location::Location(const Location &locationREF): ServerConfig(locationREF)
{
	this->ServerConfig::Location::operator=(locationREF);
};

ServerConfig::Location &ServerConfig::Location::operator=(const Location &locationREF)
{
	if (this != &locationREF)
	{
		limitExcept = locationREF.limitExcept;
	}
	return (*this);
};

/* class ServerConfig::Location::LimitExcept */
ServerConfig::Location::LimitExcept::LimitExcept(void) {};

ServerConfig::Location::LimitExcept::~LimitExcept(void) {};

ServerConfig::Location::LimitExcept::LimitExcept(const LimitExcept &limitExceptREF) {
	this->ServerConfig::Location::LimitExcept::operator=(limitExceptREF);
};

ServerConfig::Location::LimitExcept &ServerConfig::Location::LimitExcept::operator=(const LimitExcept &limitExceptREF) {
	if (this != &limitExceptREF)
	{
		acceptedMethods = limitExceptREF.acceptedMethods;
	}
	return (*this);
};
