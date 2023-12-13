/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 20:29:22 by jyao              #+#    #+#             */
/*   Updated: 2023/12/13 10:25:20 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include 	"CGIhandler.hpp"
#include 	"Header.hpp"
#include	"ToString.tpp"
#include	"ServerParser_namespace.hpp"
#include	<cstring>
#include	<cstdlib>

using namespace http;


CGIhandler::CGIhandler(void): _childPid(-1), _inFile(NULL), _inFileFd(-1), _outFile(NULL), _outFileFd(-1), _cinSave(-1), _coutSave(-1) {
	_cgiEnv["GATEWAY_INTERFACE"]	= GATEWAY_INTERFACE;
	_cgiEnv["SERVER_PROTOCOL"]		= SERVER_PROTOCOL;
	_cgiEnv["SERVER_SOFTWARE"]		= SERVER_SOFTWARE;
};

static std::string	getQueryString(const http::Request &requestREF)
{
	std::pair< std::string, std::string >	queryPair;

	queryPair = ServerParser::splitByTwo(requestREF.getUri(), QUERY_DELIM);
	switch	(requestREF.getHttpMethodEnum())
	{
		case (HEAD):
		case (GET):
			return (queryPair.second);
		case (POST):
			return (requestREF.getMessageBody());
		default :
			return ("");
	}
}

static std::string getCgiPathInfo(const http::Request &requestREF)
{
	std::string uri = requestREF.getUri();

	// Find the position of '?' to separate the URI and query string
	size_t queryPos = uri.find('?');
	std::string pathInfo;

		// Extract PATH_INFO from URI before '?'
	if (queryPos != std::string::npos)
		pathInfo = uri.substr(0, queryPos);
	// No query string, use the whole URI as PATH_INFO
	else
		pathInfo = uri;
	// Perform additional processing if needed (e.g., URL decoding)
	return pathInfo;
}

CGIhandler::CGIhandler(const http::Request &requestREF, const ServerConfig::Location *locationPTR)
{
	(void) locationPTR;
	*this = CGIhandler();
	_cgiRequest = requestREF;
	_cgiEnv["AUTH_TYPE"] 			= requestREF.getHeaderValue(HEADER_KEY_AUTH);
	_cgiEnv["CONTENT_LENGTH"]		= http::toString(requestREF.getMessageBody().size());
	_cgiEnv["CONTENT_TYPE"]			= requestREF.getHeaderValue(HEADER_KEY_CONTENT_TYPE);
	_cgiEnv["PATH_INFO"]			= getCgiPathInfo(requestREF);
	_cgiEnv["QUERY_STRING"]			= getQueryString(requestREF);
	_cgiEnv["REQUEST_METHOD"]		= requestREF.getHttpMethod();
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
		_childPid = cgiREF._childPid;
		_inFile = cgiREF._inFile;
		_inFileFd = cgiREF._inFileFd;
		_outFile = cgiREF._outFile;
		_outFileFd = cgiREF._outFileFd;
		_cinSave = cgiREF._cinSave;
		_coutSave = cgiREF._coutSave;
		_startTime = cgiREF._startTime;
		_clientSocket = cgiREF._clientSocket;
		_serverSocket = cgiREF._serverSocket;
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
	int			i = 0;
	std::string	cgiElment;

	if (cgiEnvREF.size() <= 0)
		return (NULL);
	try
	{
		envArr = new char *[cgiEnvREF.size() + 1];
		memset(envArr, 0, (cgiEnvREF.size() + 1) * sizeof(char *));
		for (std::map<std::string, std::string>::const_iterator cit = cgiEnvREF.begin(); cit != cgiEnvREF.end(); ++cit)
		{
			cgiElment = cit->first + "=" + cit->second;
			envArr[i] = new char[cgiElment.size() + 1];
			std::strcpy(envArr[i], cgiElment.c_str());
			envArr[i][cgiElment.size()] = '\0';
			++i;
		}
		envArr[i] = NULL;
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

static void	createTmpFiles(FILE **inFile, FILE **outFile, int &inFileFd, int &outFileFd) {
	*inFile = tmpfile();
	*outFile = tmpfile();
	if (*inFile == NULL || *outFile == NULL)
		throw(CGIhandler::CGIexception("CGI failed to create temporary files!"));
	inFileFd = fileno(*inFile);
	outFileFd = fileno(*outFile);
	if (inFileFd < 0 || outFileFd < 0)
		throw(CGIhandler::CGIexception("CGI failed to get fd of temporary files!"));
};

static void CGIchild(const int &cinSave, const int &coutSave, const int &inFileFd, const int &outFileFd, FILE *inFile, FILE *outFile, char *const *cgiEnv, const std::string &scriptName)
{
	if (cgiEnv != NULL)
	{
		dup2(inFileFd, STDIN_FILENO);
		dup2(outFileFd, STDOUT_FILENO);
		close(inFileFd);
		close(outFileFd);
		close(cinSave);
		close(coutSave);
		fclose(inFile);
		fclose(outFile);
		char **av = new char *[2];
		av[0] = new char[2 + scriptName.size() + 1];
		av[1] = new char[1];
		std::string src = "./" + scriptName;
		const char *csrc = src.c_str();
		memcpy(av[0], csrc, src.size());
		av[0][src.size()] = 0;
		av[1] = 0;
		execve(scriptName.c_str(), av, cgiEnv);
	}
	deleteEnvArr(cgiEnv);
}

std::string CGIhandler::executeCGI(const std::string &scriptName)
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
	createTmpFiles(&inFile, &outFile, inFileFd, outFileFd);
	write(inFileFd, _cgiRequest.getMessageBody().c_str(), _cgiRequest.getMessageBody().size());
	lseek(inFileFd, 0, SEEK_SET);
	_startTime = std::clock();
	pid = fork();
	if (pid < 0)
		throw(CGIexception("CGI failed to create fork!"));
	if (pid == 0)
		CGIchild(cinSave, coutSave, inFileFd, outFileFd, inFile, outFile, mapToArr(_cgiEnv), scriptName);
	else
	{
		_childPid = pid;
		_inFile = inFile;
		_inFileFd = inFileFd;
		_outFile = outFile;
		_outFileFd = outFileFd;
		_cinSave = cinSave;
		_coutSave = coutSave;
		usleep(100);
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

void CGIhandler::setServerSocket(int serverSocket)
{
	_serverSocket = serverSocket;
}

void CGIhandler::setClientSocket(int clientSocket)
{
	_clientSocket = clientSocket;
}