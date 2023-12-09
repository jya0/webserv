/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:57:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/09 03:00:45 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<sstream>
#include	<cstdlib>
#include	<algorithm>
#include	"ServerConfig.hpp"

/* class ServerConfig */
ServerConfig::ServerConfig(void):	_listen(std::make_pair(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT)), 
									_autoIndex(DEFAULT_AUTO_INDEX), 
									_sizeCMB(DEFAULT_CMB_SIZE),
									_index(std::vector< std::string >(1, DEFAULT_INDEX)), 
									_return(std::make_pair(DEFAULT_RETURN_CODE, DEFAULT_RETURN_URI)),
									_root(DEFAULT_ROOT) {};

ServerConfig::~ServerConfig(void) {};

static std::pair< std::string, int >	parseListenDirective(std::string listenDve) {
	std::stringstream	ssListen;
	std::string			ipStr;
	std::string			portStr;
	int					port;

	ssListen.str(listenDve);
	std::getline(ssListen, ipStr, ':');
	ssListen >> portStr;
	ipStr = (ipStr.empty()) ? DEFAULT_LISTEN_IP : ipStr;
	port = (portStr.empty()) ? DEFAULT_LISTEN_PORT : strtol(portStr.c_str(), NULL, 10);
	return (std::make_pair(ipStr, port));
};

static std::pair< int, std::string >	parseReturnDirective(std::string returnDve) {
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

static std::vector< t_errorPage >	parseErrorPages(const std::vector< std::vector< std::string > > &errorPages) {
	std::vector< t_errorPage >	parsedErrorPages;
	t_errorPage					parsedErrorPage;
	std::stringstream			ss;
	int							code;

	for (std::vector< std::vector< std::string > >::const_iterator itc1 = errorPages.begin(); itc1 != errorPages.end(); ++itc1)
	{
		parsedErrorPage.second = itc1->back();
		for (std::vector< std::string >::const_iterator itc2 = itc1->begin(); itc2 != (itc1->end() - 1); ++itc2)
		{
			ss.str(*itc2);
			ss.clear();
			ss >> code;
			parsedErrorPage.first.push_back(code);
		};
		parsedErrorPages.push_back(parsedErrorPage);
	};
	return (parsedErrorPages);
};

ServerConfig::ServerConfig(DirectiveBlock *serverDve) {
	*this = ServerConfig();
	if (serverDve != NULL)
	{
		try {
			_listen = parseListenDirective(serverDve->readDirectiveSimple(DVE_LISTEN).front());
		} catch (std::exception &e) {}
		try {
			_serverNames = serverDve->readDirectiveSimple(DVE_SERVER_NAME);
		} catch (std::exception &e) {}
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
			_autoIndex = (serverDve->readDirectiveSimple(DVE_AUTO_INDEX).front() == "on") ? true : false;
		} catch (std::exception &e) {};
		try {
			_sizeCMB = std::atof(serverDve->readDirectiveSimple(DVE_CMB_SIZE).front().c_str());
		} catch (std::exception &e) {};
		try {
			_errorPages = parseErrorPages(serverDve->readDirectivesSimple(DVE_ERROR_PAGE));
		} catch (std::exception &e) {};
		try {
			_index = serverDve->readDirectiveSimple(DVE_INDEX);
		} catch (std::exception &e) {};
		try {
			_return = parseReturnDirective(serverDve->readDirectiveSimple(DVE_RETURN).front());
		} catch (std::exception &e) {};
		try {
			_root = serverDve->readDirectiveSimple(DVE_ROOT).front();
		} catch (std::exception &e) {};
	};
};

ServerConfig::ServerConfig(const ServerConfig &serverConfigREF) {
	this->ServerConfig::operator=(serverConfigREF);
};

ServerConfig &ServerConfig::operator=(const ServerConfig &serverConfigREF) {
	if (this != &serverConfigREF)
	{
		_listen			= serverConfigREF.getListen();
		_serverNames	= serverConfigREF.getServerNames();
		_locations		= serverConfigREF.getLocations();
		_autoIndex		= serverConfigREF.getAutoIndex();
		_sizeCMB		= serverConfigREF.getSizeCMB();
		_errorPages		= serverConfigREF.getErrorPages();
		_index			= serverConfigREF.getIndex();
		_return			= serverConfigREF.getReturn();
		_root			= serverConfigREF.getRoot();
	}
	return (*this);
};

const std::vector< std::string >						&ServerConfig::getIndex(void) const {
	return (_index);
};

const std::pair< std::string, int >						&ServerConfig::getListen(void) const {
	return (_listen);
};

const std::vector< std::string >						&ServerConfig::getServerNames(void) const {
	return (_serverNames);
};

const std::vector< ServerConfig::Location >				&ServerConfig::getLocations(void) const {
	return (_locations);
};

std::vector< ServerConfig::Location >::const_iterator	ServerConfig::getLocation(const std::string &uriREF) const {
	std::vector< Location >::const_iterator	itc;

	itc = std::find_if(_locations.begin(), _locations.end(), Location::IsLocationUnary(uriREF));
	return (itc);
};

const bool												&ServerConfig::getAutoIndex(void) const {
	return (_autoIndex);
};

const std::size_t										&ServerConfig::getSizeCMB(void) const {
	return (_sizeCMB);
};

const std::vector< t_errorPage >						&ServerConfig::getErrorPages(void) const {
	return (_errorPages);
};

std::string												ServerConfig::getErrorPage(const int &statusCode) const {
	for (std::vector< t_errorPage >::const_iterator itc1 = _errorPages.begin(); itc1 != _errorPages.end(); ++itc1)
	{
		for (std::vector< int >::const_iterator itc2 = itc1->first.begin(); itc2 != itc1->first.end(); ++itc2)
		{
			if (statusCode == *itc2)
				return (itc1->second);
		}
	}
	return ("");
};

const std::pair< int, std::string >						&ServerConfig::getReturn(void) const {
	return (_return);
};

const std::string										&ServerConfig::getRoot(void) const {
	return (_root);
};

/* class ServerConfig::Location */
ServerConfig::Location::Location(void): ServerConfig() {};

ServerConfig::Location::Location(ADirective *locationDve): ServerConfig(dynamic_cast<DirectiveBlock *>(locationDve)) {
	DirectiveBlock	*dveBlockPTR;

	if (locationDve != NULL)
	{
		locationUri	= locationDve->getValues().front();
		try {
			dveBlockPTR = dynamic_cast<DirectiveBlock *>(locationDve);
			limitExcept = (dveBlockPTR == NULL) ? LimitExcept() : LimitExcept(dveBlockPTR->readDirectiveSimple(DVE_LIMIT_EXECPT));
		} catch (std::exception &e) {};
	};
};

ServerConfig::Location::~Location(void) {};

ServerConfig::Location::Location(const Location &locationREF): ServerConfig(locationREF) {
	this->ServerConfig::Location::operator=(locationREF);
};

ServerConfig::Location &ServerConfig::Location::operator=(const Location &locationREF) {
	if (this != &locationREF)
	{
		locationUri = locationREF.locationUri;
		limitExcept = locationREF.limitExcept;
	}
	return (*this);
};

/* class ServerConfig::Location::LimitExcept */
ServerConfig::Location::LimitExcept::LimitExcept(void): acceptedMethods(DEFAULT_LIMIT_EXCEPT_METHODS) {};

ServerConfig::Location::LimitExcept::LimitExcept(const std::vector< std::string > &methodStrs) {
	if (!methodStrs.empty())
	{
		for (std::vector< std::string >::const_iterator itr = methodStrs.begin(); itr != methodStrs.end(); ++itr)
		{
			acceptedMethods |= http::Request::methodEnum(*itr);
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
