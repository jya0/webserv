/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 20:29:22 by jyao              #+#    #+#             */
/*   Updated: 2023/12/15 00:50:13 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include 	"CGIhandler.hpp"
#include 	"Header.hpp"
#include	"ToString.tpp"
#include	"ServerParser_namespace.hpp"
#include	<cstring>
#include	<cstdlib>

using namespace http;

PollManager *CGIhandler::sockets = NULL;

CGIhandler::CGIhandler(void):	_childPid(-1), _inFile(NULL), _inFileFd(-1),
								_outFile(NULL), _outFileFd(-1), _cinSave(-1), _coutSave(-1),
								_clientSocket(-1), _serverSocket(-1) {
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
			return (requestREF.getMessageBodyStr());
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
	_cgiEnv["CONTENT_LENGTH"]		= http::toString(requestREF.getMessageBodyStr().size());
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

void	CGIhandler::deleteEnvArr(char * const *envArr) {
	if (envArr != NULL)
	{
		for (int i = 0; envArr[i] != NULL; ++i)
			delete (envArr[i]);
		delete[] (envArr);
	}
};

char	**CGIhandler::mapToArr(const std::map< std::string, std::string > &cgiEnvREF) {
	char		**envArr;
	int			i = 0;
	std::string	cgiElment;

	envArr = NULL;
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

void CGIhandler::createTmpFiles()
{
	_inFile = tmpfile();
	_outFile = tmpfile();
	if (_inFile == NULL || _outFile == NULL)
		throw(CGIhandler::CGIexception("CGI failed to create temporary files!"));
	_inFileFd = fileno(_inFile);
	_outFileFd = fileno(_outFile);
	if (_inFileFd < 0 || _outFileFd < 0)
		throw(CGIhandler::CGIexception("CGI failed to get fd of temporary files!"));
};

void CGIhandler::runCGI(const std::string &scriptName)
{
	int status = 0;
	char **envp = mapToArr(_cgiEnv);
	closeChildFds();
	char **av = new char *[2];
	av[0] = new char[2 + scriptName.size() + 1];
	av[1] = new char[1];
	std::string src = "./" + scriptName;
	const char *csrc = src.c_str();
	memcpy(av[0], csrc, src.size());
	av[0][src.size()] = 0;
	av[1] = 0;
	status = execve(scriptName.c_str(), av, envp);
	deleteEnvArr(envp);
	exit(status);
}

void CGIhandler::closeChildFds() {
	dup2(_inFileFd, STDIN_FILENO);
	dup2(_outFileFd, STDOUT_FILENO);
	close(_cinSave);
	close(_coutSave);
	close(_inFileFd);
	close(_outFileFd);
	fclose(_inFile);
	fclose(_outFile);
	for (int i = 0; i < CGIhandler::sockets->getNfds(); i++)
		close((*CGIhandler::sockets)[i].fd);
}

void CGIhandler::closeParentFds() const {
	dup2(_cinSave, STDIN_FILENO);
	dup2(_coutSave, STDOUT_FILENO);
	close(_cinSave);
	close(_coutSave);
	close(_inFileFd);
	close(_outFileFd);
	fclose(_inFile);
	fclose(_outFile);

}


void CGIhandler::executeCGI(const std::string &scriptName)
{
	_cinSave = dup(STDIN_FILENO);
	_coutSave = dup(STDOUT_FILENO);
	createTmpFiles();
	if (write(_inFileFd, _cgiRequest.getMessageBodyStr().c_str(), _cgiRequest.getMessageBodyStr().size()) < 0)
		std::cerr	<< "Failed to write"
					<< std::endl;
	lseek(_inFileFd, 0, SEEK_SET);
	_startTime = std::clock();
	_childPid = fork();
	if (_childPid < 0)
		throw(500);
	if (_childPid == 0)
		runCGI(scriptName);
	else
		throw(*this);
};

const std::clock_t &CGIhandler::getStartTime() const
{
	return (_startTime);
}

const pid_t &CGIhandler::getChildPid() const
{
	return (_childPid);
}

const int &CGIhandler::getCinSave() const
{
	return (_cinSave);
}

const int &CGIhandler::getCoutSave() const
{
	return (_coutSave);
}

const int	&CGIhandler::getInFileFd() const
{
	return (_inFileFd);
}

const int	&CGIhandler::getOutFileFd() const
{
	return (_outFileFd);
}

const FILE	*CGIhandler::getInFile() const
{
	return (_inFile);
}

const FILE	*CGIhandler::getOutFile() const
{
	return (_outFile);
}

const int	&CGIhandler::getClientSocket() const{
	return (_clientSocket);
}

const int	&CGIhandler::getServerSocket() const{
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

void CGIhandler::setPollManager(PollManager &socketsREF)
{
	CGIhandler::sockets = &socketsREF;
}
