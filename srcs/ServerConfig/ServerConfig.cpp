/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:57:39 by jyao              #+#    #+#             */
/*   Updated: 2023/11/18 19:51:37 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<sstream>
#include	"ServerConfig.hpp"

/* class ServerConfig */
ServerConfig::ServerConfig(void):	_cgiBin(DEFAULT_CGI_BIN), 
									_listen(std::make_pair(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT)), 
									_autoIndex(DEFAULT_AUTO_INDEX), 
									_sizeCMB(DEFAULT_CMB_SIZE), 
									_errorPage(DEFAULT_ERROR_PAGE), 
									_index(std::vector< std::string >(1, DEFAULT_INDEX)), 
									_return(std::make_pair(DEFAULT_RETURN_CODE, DEFAULT_RETURN_URI)),
									_root(DEFAULT_ROOT) {};

ServerConfig::~ServerConfig(void) {};

static std::pair< std::string, int >	parseListenDirective(std::string listenDve)
{
	std::stringstream	tmpSstream;

	tmpSstream.str(listenDve);
	std::replace(listenDve.begin(), listenDve.end(), )
};

ServerConfig::ServerConfig(DirectiveBlock *serverDve)
{
	*this = ServerConfig();
	if (serverDve != NULL)
	{
		try {
			_index = serverDve->checkDirectiveSimple(DVE_INDEX);
		} catch (std::exception &e) {};
		try {
			_cgiBin	= serverDve->checkDirectiveSimple(DVE_CGI_BIN).front();
		} catch (std::exception &e) {};
		try {
			_listen.first = serverDve->checkDirectiveSimple(DVE_LISTEN).front();
		} catch (std::exception &e) {};
		try {
			_serverNames = serverDve->checkDirectiveSimple(DVE_SERVER_NAME);
		} catch (std::exception &e) {};
		try {
			std::pair< std::multimap< std::string, ADirective * >::const_iterator, std::multimap< std::string, ADirective * >::const_iterator>	dveITRS;

			dveITRS = serverDve->getDvesMap()->equal_range(DVE_LOCATION);
			while (dveITRS.first != dveITRS.second)
			{
				_locations.push_back(Location(dveITRS.first->second));
				dveITRS.first++;
			}
		} catch (std::exception &e) {};
		try {
			_autoIndex = serverDve->checkDirectiveSimple(DVE_AUTO_INDEX).front();
		}
	};
};

ServerConfig::ServerConfig(const ServerConfig &serverConfigREF)
{
	this->ServerConfig::operator=(serverConfigREF);
};

ServerConfig &ServerConfig::operator=(const ServerConfig &serverConfigREF)
{
	if (this != &serverConfigREF)
	{
		_cgiBin			= serverConfigREF.getCgiBin();
		_listen			= serverConfigREF.getListen();
		_serverNames	= serverConfigREF.getServerNames();
		_locations		= serverConfigREF.getLocations();
		_autoIndex		= serverConfigREF.getAutoIndex();
		_sizeCMB		= serverConfigREF.getSizeCMB();
		_errorPage		= serverConfigREF.getErrorPage();
		_index			= serverConfigREF.getIndex();
		_return			= serverConfigREF.getReturn();
		_root			= serverConfigREF.getRoot();
	}
	return (*this);
};

std::string	ServerConfig::getCgiBin(void) const {
	return (_cgiBin);
};

std::vector< std::string >	ServerConfig::getIndex(void) const {
	return (_index);
};

std::pair< std::string, int >	ServerConfig::getListen(void) const {
	return (_listen);
};

std::vector< std::string >	ServerConfig::getServerNames(void) const {
	return (_serverNames);
};

std::vector< ServerConfig::Location >		ServerConfig::getLocations(void) const {
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

std::pair< int, std::string >	ServerConfig::getReturn(void) const {
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



ServerConfig::Location::Location(DirectiveBlock *locationDve): ServerConfig(locationDve)
{
	if (locationDve != NULL)
	{
		locationUri	= locationDve->getValues().front();
		try {
			limitExcept = LimitExcept(locationDve->checkDirectiveBlock(DVE_LIMIT_EXECPT));
		} catch (std::exception &e) {};
	};
};

ServerConfig::Location::~Location(void) {};

ServerConfig::Location::Location(const Location &locationREF): ServerConfig(locationREF)
{
	this->ServerConfig::Location::operator=(locationREF);
};

ServerConfig::Location &ServerConfig::Location::operator=(const Location &locationREF)
{
	if (this != &locationREF)
	{
		locationUri = locationREF.locationUri;
		limitExcept = locationREF.limitExcept;
	}
	return (*this);
};

/* class ServerConfig::Location::LimitExcept */
ServerConfig::Location::LimitExcept::LimitExcept(void) {};

ServerConfig::Location::LimitExcept::LimitExcept(DirectiveBlock *limitExceptDve)
{
	std::vector< std::string >	methodStrs;

	if (limitExceptDve != NULL)
	{
		methodStrs = limitExceptDve->getValues();
		for (std::vector< std::string >::const_iterator itr = methodStrs.begin(); itr != methodStrs.end(); ++itr)
		{
			acceptedMethods |= Request::methodEnum(*itr);
		}
	};
};

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
