/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:57:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/12 20:22:50 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<sstream>
#include	<cstdlib>
#include	<algorithm>
#include	"ServerConfig.hpp"
#include	"Response.hpp"
#include	"ServerParser_namespace.hpp"

/* class ServerConfig */
ServerConfig::ServerConfig(void):	_listen(std::make_pair(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT)),
									_autoIndex(DEFAULT_AUTO_INDEX),
									_sizeCMB(DEFAULT_CMB_SIZE),
									_index(std::vector< std::string >(1, DEFAULT_INDEX)),
									_root(DEFAULT_ROOT) {};

ServerConfig::~ServerConfig(void) {};

static std::pair< std::string, int >	parseListenDirective(std::string listenDve) {
	std::pair< std::string, std::string >	listenPair;
	std::string								ipStr;
	std::string								portStr;
	int										port;

	listenPair = ServerParser::splitByTwo(listenDve, ':');
	ipStr = listenPair.first;
	ipStr = (ipStr.empty()) ? DEFAULT_LISTEN_IP : ipStr;
	portStr = listenPair.second;
	port = (portStr.empty()) ? DEFAULT_LISTEN_PORT : strtol(portStr.c_str(), NULL, 10);
	return (std::make_pair(ipStr, port));
};

static ServerConfig::Return	parseReturnDirective(std::vector< std::string > returnDve) {
	ServerConfig::Return	returnObj;

	returnObj.isInit = true;
	returnObj.code = (returnDve.size() > 1) ? std::atoi(returnDve.front().c_str()) : DEFAULT_RETURN_CODE;
	returnObj.uri = returnDve.back();
	return (returnObj);
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
			_sizeCMB = std::atol(serverDve->readDirectiveSimple(DVE_CMB_SIZE).front().c_str());
		} catch (std::exception &e) {};
		try {
			_errorPages = parseErrorPages(serverDve->readDirectivesSimple(DVE_ERROR_PAGE));
		} catch (std::exception &e) {};
		try {
			_index = serverDve->readDirectiveSimple(DVE_INDEX);
		} catch (std::exception &e) {};
		try {
			_return = parseReturnDirective(serverDve->readDirectiveSimple(DVE_RETURN));
		} catch (std::exception &e) {};
		try {
			_root = ServerParser::appendSlashes(serverDve->readDirectiveSimple(DVE_ROOT).front());
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

const ServerConfig::Location	*ServerConfig::getLocation(const std::string &uriREF) const {
	std::vector< Location >::const_iterator	itc;

	itc = std::find_if(_locations.begin(), _locations.end(), Location::IsLocationUnary(uriREF));
	if (itc == _locations.end() && http::checkMimeType(uriREF) == MIME_CGI)
		itc = std::find_if(_locations.begin(), _locations.end(), Location::IsLocationUnary(uriREF.substr(0, uriREF.rfind('.'))));
	return ((itc == _locations.end()) ? NULL : &(*itc));
};

const bool												&ServerConfig::getAutoIndex(void) const {
	return (_autoIndex);
};

const ssize_t											&ServerConfig::getSizeCMB(void) const {
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

const ServerConfig::Return								&ServerConfig::getReturn(void) const {
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
		locationUri = ServerParser::appendSlashes(locationUri);
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

ServerConfig::Return::Return(void): isInit(false), code(DEFAULT_RETURN_CODE), uri(DEFAULT_RETURN_URI) {};

ServerConfig::Return::~Return(void) {};

ServerConfig::Return::Return(const Return &returnREF) {
	this->operator=(returnREF);
};

ServerConfig::Return	&ServerConfig::Return::operator=(const Return &returnREF) {
	if (this != &returnREF)
	{
		isInit = returnREF.isInit;
		code = returnREF.code;
		uri = returnREF.uri;
	}
	return (*this);
};

/* class ServerConfig::Location::LimitExcept */
ServerConfig::Location::LimitExcept::LimitExcept(void): acceptedMethods(DEFAULT_LIMIT_EXCEPT_METHODS) {};

ServerConfig::Location::LimitExcept::LimitExcept(const std::vector< std::string > &methodStrs) {
	acceptedMethods = 0;
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
