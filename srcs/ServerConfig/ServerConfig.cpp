/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:57:39 by jyao              #+#    #+#             */
/*   Updated: 2023/11/19 13:32:38 by jyao             ###   ########.fr       */
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
	std::stringstream	ssListen;
	std::string			ipStr;
	std::string			portStr;
	int					port;

	{
		if (std::count(listenDve.begin(), listenDve.end(), ":") > 1)
			std::cout << "DO YOU REMEMBER?" << std::endl;
	}
	ssListen.str(listenDve);
	std::getline(ssListen, ipStr, ':');
	ssListen >> portStr;
	ipStr = (ipStr.empty()) ? DEFAULT_LISTEN_IP : ipStr;
	port = (portStr.empty()) ? DEFAULT_LISTEN_PORT : std::atoi(portStr.c_str());
	return (std::make_pair(ipStr, port));
};

static std::pair< int, std::string >	parseReturnDirective(std::string returnDve)
{
	std::stringstream	ssReturn;
	std::string			returnCodeStr;
	int					returnCode;
	std::string			returnUriStr;

	ssReturn.str(returnDve);
	ssReturn >> returnCodeStr;
	ssReturn >> returnUriStr;
	returnCode = (returnCodeStr.empty()) ? DEFAULT_RETURN_CODE : std::atoi(returnCodeStr.c_str());
	returnUriStr = (returnUriStr.empty()) ? DEFAULT_RETURN_URI : returnUriStr;
	return (std::make_pair(returnCode, returnUriStr));
};

ServerConfig::ServerConfig(DirectiveBlock *serverDve)
{
	*this = ServerConfig();
	if (serverDve != NULL)
	{
		try {
			_cgiBin	= serverDve->checkDirectiveSimple(DVE_CGI_BIN).front();
		} catch (std::exception &e) {};
		try {
			_listen = parseListenDirective(serverDve->checkDirectiveSimple(DVE_LISTEN).front());
		} catch (std::exception &e) {};
		try {
			_serverNames = serverDve->checkDirectiveSimple(DVE_SERVER_NAME);
		} catch (std::exception &e) {};
		try {
			std::pair< std::multimap< std::string, ADirective * >::const_iterator, std::multimap< std::string, ADirective * >::const_iterator>	dveITRS;
			DirectiveBlock	*dveBlock;

			dveITRS = serverDve->getDvesMap()->equal_range(DVE_LOCATION);
			while (dveITRS.first != dveITRS.second)
			{
				_locations.push_back(Location(dveITRS.first->second));
				dveITRS.first++;
			}
		} catch (std::exception &e) {};
		try {
			_autoIndex = serverDve->checkDirectiveSimple(DVE_AUTO_INDEX).front().compare("on") ? false : true;
		} catch (std::exception &e) {};
		try {
			_sizeCMB = std::atof(serverDve->checkDirectiveSimple(DVE_CMB_SIZE).front().c_str());
		} catch (std::exception &e) {};
		try {
			_errorPage = serverDve->checkDirectiveSimple(DVE_ERROR_PAGE).front();
		} catch (std::exception &e) {};
		try {
			_index = serverDve->checkDirectiveSimple(DVE_INDEX);
		} catch (std::exception &e) {};
		try {
			_return = parseReturnDirective(serverDve->checkDirectiveSimple(DVE_RETURN).front());
		} catch (std::exception &e) {};
		try {
			_root = serverDve->checkDirectiveSimple(DVE_ROOT).front();
		} catch (std::exception &e) {};
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

/* class ServerConfig::Location */
ServerConfig::Location::Location(void): ServerConfig() {};

ServerConfig::Location::Location(ADirective *locationDve): ServerConfig(dynamic_cast<DirectiveBlock *>(locationDve))
{
	DirectiveBlock	*dveBlockPTR;

	if (locationDve != NULL)
	{
		locationUri	= locationDve->getValues().front();
		try {
			dveBlockPTR = dynamic_cast<DirectiveBlock *>(locationDve);
			limitExcept = (dveBlockPTR == NULL) ? LimitExcept() : LimitExcept(dveBlockPTR->checkDirectiveBlock(DVE_LIMIT_EXECPT));
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
ServerConfig::Location::LimitExcept::LimitExcept(void): acceptedMethods(DEFAULT_LIMIT_EXCEPT_METHODS) {};

ServerConfig::Location::LimitExcept::LimitExcept(ADirective *limitExceptDve)
{
	std::vector< std::string >	methodStrs;

	*this = LimitExcept();
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
