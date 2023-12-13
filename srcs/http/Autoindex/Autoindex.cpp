/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:04 by jyao              #+#    #+#             */
/*   Updated: 2023/12/13 06:21:23 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<dirent.h>
#include	<sys/stat.h>
#include	<iostream>
#include	<sstream>
#include	<cstdlib>
#include	"Autoindex_namespace.hpp"
#include	"ServerParser_namespace.hpp"

using namespace http;

static std::string	direntLink(const std::string &uri, const std::string &dirent, const std::string &hostREF, const int &portREF) {
	std::stringstream	ssDirentLink;

	ssDirentLink << "\t\t<p><a href=\"http://" + hostREF + ":" << portREF << uri + dirent + "\">" + dirent + "</a></p>\n";
	return (ssDirentLink.str());
};

static void	pageAddOpening(std::string &pageREF, const std::string &dirNameREF) {
	pageREF +=	"<!DOCTYPE html>\n"
					"<html>\n"
						"<head>\n"
	        				"<title>" + dirNameREF + "</title>\n"
						"</head>\n"
						"<body>\n"
							"<h1>INDEX</h1>\n"
							"<p>\n";
};

static void	pageAddClosing(std::string &pageREF) {
	pageREF +=	"</p>\n"
				"</body>\n"
    			"</html>\n";
};

std::string	Autoindex::genPage(std::string path, const Request &requestREF) {
	DIR										*dirPTR;
	std::string								page;
	std::string								direntName;
	std::string								uri;
	std::pair< std::string, std::string >	hostNport;

	if (path.empty())
		return (page);
	path = ServerParser::appendSlashes(path);
	uri = ServerParser::appendSlashes(requestREF.getUri());
	dirPTR = opendir(path.c_str());
	if (dirPTR == NULL)
	{
		std::cerr	<< "Couldn't open directory: "
					<< path
					<< std::endl;

		return (page);
	};
	hostNport = ServerParser::splitByTwo(requestREF.getHeaderValue("Host"), ':');
	pageAddOpening(page, requestREF.getUri());
	for (struct dirent *dirent = readdir(dirPTR); dirent != NULL; dirent = readdir(dirPTR))
	{
		direntName = dirent->d_name;
		if (isPathFolder(path + direntName) > 0)
			direntName += "/";
		page += direntLink(uri, direntName, hostNport.first, std::atoi(hostNport.second.c_str()));
	}
	closedir(dirPTR);
	pageAddClosing(page);
	return (page);
};

int	Autoindex::isPathFolder(const std::string &path) {
	struct stat	pathStat;

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (pathStat.st_mode & S_IFDIR);
};

int	Autoindex::isPathReg(const std::string &path) {
	struct stat	pathStat;

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (pathStat.st_mode & S_IFREG);
};

int	Autoindex::isPathExec(const std::string &path) {
	struct stat	pathStat;

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (pathStat.st_mode & S_IXUSR);
};

int	Autoindex::isPathRead(const std::string &path) {
	struct stat	pathStat;

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (pathStat.st_mode & S_IRUSR);
};

int	Autoindex::isPathWrite(const std::string &path) {
	struct stat	pathStat;

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (pathStat.st_mode & S_IWUSR);
};

int	Autoindex::isPathExist(const std::string &path) {
	struct stat	pathStat;

	return ((stat(path.c_str(), &pathStat) == 0) ? 1 : 0);
};
