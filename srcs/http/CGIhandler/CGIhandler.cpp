/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 20:29:22 by jyao              #+#    #+#             */
/*   Updated: 2023/12/04 04:15:15 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CGIhandler.hpp"
#include "Header.hpp"

using namespace http;

/*
std::map<std::string, std::string>	headers = request.getHeaders();
if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
	this->_env["AUTH_TYPE"] = headers["Authorization"];

this->_env["REDIRECT_STATUS"] = "200"; //Security needed to execute php-cgi
this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
this->_env["SCRIPT_NAME"] = config.getPath();
this->_env["SCRIPT_FILENAME"] = config.getPath();
this->_env["REQUEST_METHOD"] = request.getMethod();
this->_env["CONTENT_LENGTH"] = to_string(this->_body.length());
this->_env["CONTENT_TYPE"] = headers["Content-Type"];
this->_env["PATH_INFO"] = request.getPath(); //might need some change, using config path/contentLocation
this->_env["PATH_TRANSLATED"] = request.getPath(); //might need some change, using config path/contentLocation
this->_env["QUERY_STRING"] = request.getQuery();
this->_env["REMOTEaddr"] = to_string(config.getHostPort().host);
this->_env["REMOTE_IDENT"] = headers["Authorization"];
this->_env["REMOTE_USER"] = headers["Authorization"];
this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
if (headers.find("Hostname") != headers.end())
	this->_env["SERVER_NAME"] = headers["Hostname"];
else
	this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];
this->_env["SERVER_PORT"] = to_string(config.getHostPort().port);
this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
this->_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
*/

CGIhandler::CGIhandler(void): _childPid(-1), _inFile(NULL), _inFileFd(-1), _outFile(NULL), _outFileFd(-1), _cinSave(-1), _coutSave(-1) {
	_cgiEnv["GATEWAY_INTERFACE"]	= GATEWAY_INTERFACE;
	_cgiEnv["SERVER_PROTOCOL"]		= SERVER_PROTOCOL;
	_cgiEnv["SERVER_SOFTWARE"]		= SERVER_SOFTWARE;
};

CGIhandler::CGIhandler(const http::Request &requestREF, const ServerConfig::Location &locationREF) {
	*this = CGIhandler();
	(void)locationREF;
	_cgiRequest = requestREF;
	_cgiEnv["AUTH_TYPE"] 			= requestREF.getHeaderValue(HEADER_KEY_AUTH);
	_cgiEnv["CONTENT_LENGTH"]		= std::to_string(requestREF.getMessageBody().size());
	_cgiEnv["CONTENT_TYPE"]			= requestREF.getHeaderValue(HEADER_KEY_CONTENT_TYPE);
	// _cgiEnv["PATH_INFO"]			= requestREF.getHeaderValue(locationREF.getCgiPathInfo());
	// _cgiEnv["PATH_TRANSLATED"]		= requestREF.getHeaderValue(locationREF.getCgiPathInfo());
	// _cgiEnv["QUERY_STRING"]			= requestREF.getHeaderValue(requestREF.getQuery());
	// _cgiEnv["REMOTE_ADDR"]			= requestREF.getHeaderValue();
	// _cgiEnv["REMOTE_HOST"]			= requestREF.getHeaderValue();
	// _cgiEnv["REMOTE_IDENT"]			= requestREF.getHeaderValue();
	// _cgiEnv["REMOTE_USER"]			= requestREF.getHeaderValue();
	_cgiEnv["REQUEST_METHOD"] = requestREF.getHttpMethod();
	// _cgiEnv["SCRIPT_NAME"]			= requestREF.getHeaderValue();
	// _cgiEnv["SERVER_PORT"]			= requestREF.getHeaderValue();
};

CGIhandler::CGIhandler(const CGIhandler &cgiREF) {
	this->operator=(cgiREF);
};

CGIhandler::~CGIhandler(void){};

CGIhandler	&CGIhandler::operator=(const CGIhandler &cgiREF) {
	if (this != &cgiREF)
	{
		_cgiEnv = cgiREF.getCgiEnv();
		_cgiRequest = cgiREF.getCgiRequest();
	}
	return (*this);
};

const std::map< std::string, std::string >	&CGIhandler::getCgiEnv(void) const {
	return (_cgiEnv);
};

const http::Request							&CGIhandler::getCgiRequest(void) const {
	return (_cgiRequest);
};

static void	deleteEnvArr(char * const *envArr) {
	if (envArr != NULL)
	{
		for (int i = 0; envArr[i] != NULL; ++i)
			delete (envArr[i]);
		delete[] (envArr);
	}
};

static char	**mapToArr(const std::map< std::string, std::string > &cgiEnvREF) {
	char		**envArr;
	char		*envArrPTR;
	std::string	cgiElment;

	if (cgiEnvREF.size() <= 0)
		return (NULL);
	try
	{
		envArr = new char *[cgiEnvREF.size() + 1];
		envArrPTR = *envArr;
		for (std::map<std::string, std::string>::const_iterator cit = cgiEnvREF.begin(); cit != cgiEnvREF.end(); ++cit)
		{
			cgiElment = cit->first + "=" + cit->second;
			envArrPTR = new char[cgiElment.size() + 1];
			std::strcpy(envArrPTR, cgiElment.c_str());
			envArrPTR[cgiElment.size()] = '\0';
			++envArrPTR;
		}
		envArrPTR = NULL;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what()
				  << std::endl;
		deleteEnvArr(envArr);
		return (NULL);
	}
	return (envArr);
};

static void	createTmpFiles(FILE *&inFile, FILE *&outFile, int &inFileFd, int &outFileFd) {
	inFile = tmpfile();
	outFile = tmpfile();
	if (inFile == NULL || outFile == NULL)
		throw(CGIhandler::CGIexception("CGI failed to create temporary files!"));
	inFileFd = fileno(inFile);
	outFileFd = fileno(outFile);
	if (inFileFd < 0 || outFileFd < 0)
		throw(CGIhandler::CGIexception("CGI failed to get fd of temporary files!"));
};

static void	CGIchild(const int &inFileFd, const int &outFileFd, char * const *cgiEnv, const std::string &scriptName) {
	if (cgiEnv != NULL)
	{
		dup2(inFileFd, STDIN_FILENO);
		dup2(outFileFd, STDOUT_FILENO);
		(void)inFileFd;
		(void)outFileFd;
		// int x = execl(scriptName.c_str(), scriptName.c_str());
		char **av = new char *[2];
		av[0] = new char[2 + scriptName.size() + 1];
		av[1] = new char[1];
		// av[0] = scriptName.c_str();
		std::string src = "./" + scriptName;
		const char *csrc = src.c_str();
		memcpy(av[0], csrc, src.size());
		av[0][src.size()] = 0;
		av[1] = 0;
		execve(scriptName.c_str(), av, NULL);
	}
	// deleteEnvArr(cgiEnv);
}

/*
██╗   ██╗███████╗███████╗    ██╗███╗   ██╗    ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗     ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗ ██╗
██║   ██║██╔════╝██╔════╝    ██║████╗  ██║    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗██║
██║   ██║███████╗█████╗      ██║██╔██╗ ██║    ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝██║
██║   ██║╚════██║██╔══╝      ██║██║╚██╗██║    ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗    ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗╚═╝
╚██████╔╝███████║███████╗    ██║██║ ╚████║    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║██╗
 ╚═════╝ ╚══════╝╚══════╝    ╚═╝╚═╝  ╚═══╝    ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝
  */

std::string CGIhandler::executeCGI(const std::string &scriptName) throw(std::exception, CGIhandler)
{
	pid_t pid;
	int cinSave;
	int coutSave;
	FILE *inFile;
	int inFileFd;
	FILE *outFile;
	int outFileFd;
	std::string cgiResult;

	cinSave = dup(STDIN_FILENO);
	coutSave = dup(STDOUT_FILENO);
	createTmpFiles(inFile, outFile, inFileFd, outFileFd);
	write(inFileFd, _cgiRequest.getMessageBody().c_str(), _cgiRequest.getMessageBody().size());
	lseek(inFileFd, 0, SEEK_SET);
	_startTime = std::clock_t();
	pid = fork();
	if (pid < 0)
		throw(CGIexception("CGI failed to create fork!"));
	if (pid == 0)
	{
		CGIchild(inFileFd, outFileFd, mapToArr(_cgiEnv), scriptName);
	}
	else
	{
		_childPid = pid;
		_inFile = inFile;
		_inFileFd = inFileFd;
		_outFile = outFile;
		_outFileFd = outFileFd;
		_cinSave = cinSave;
		_coutSave = coutSave;
		throw(*this);
	}
	dup2(cinSave, STDIN_FILENO);
	dup2(coutSave, STDOUT_FILENO);
	close(cinSave);
	close(coutSave);
	close(inFileFd);
	close(outFileFd);
	fclose(inFile);
	fclose(outFile);
	if (pid == 0)
		exit(EXIT_FAILURE);
	return (cgiResult);
};

std::clock_t &CGIhandler::getStartTime()
{
	return (_startTime);
}

pid_t &CGIhandler::getChildPid()
{
	return (_childPid);
}

int &CGIhandler::getCinSave()
{
	return (_cinSave);
}

int &CGIhandler::getCoutSave()
{
	return (_coutSave);
}

int &CGIhandler::getInFileFd()
{
	return (_inFileFd);
}

int &CGIhandler::getOutFileFd()
{
	return (_outFileFd);
}

FILE *CGIhandler::getInFile()
{
	return (_inFile);
}

FILE *CGIhandler::getOutFile()
{
	return (_outFile);
}

int	CGIhandler::getClientSocket() {
	return (_clientSocket);
}


int CGIhandler::getServerSocket() {
	return (_serverSocket);
}
