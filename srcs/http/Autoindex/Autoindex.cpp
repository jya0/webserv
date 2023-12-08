/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:04 by jyao              #+#    #+#             */
/*   Updated: 2023/12/07 21:35:15 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<dirent.h>
#include	<sys/stat.h>
#include	<iostream>
#include	<sstream>
#include	"Autoindex_namespace.hpp"

using namespace http;

static std::string	direntLink(const std::string &dirName, const std::string &dirent, const std::string &hostREF, const int &portREF) {
	std::stringstream	ssDirentLink;
	std::string			slash("/");

	if (!dirName.empty())
		slash = (dirName[dirName.size() - 1] == '/' ? "" : "/");
	ssDirentLink << "\t\t<p><a href=\"http://" + hostREF + ":" << portREF << dirName + (slash) + dirent + "\">" + dirent + "</a></p>\n";
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

std::string	Autoindex::genPage(const char *path, const Request &requestREF, const ServerConfig &servConfREF) {
	DIR			*dirPTR;
	std::string	page;

	if (path == NULL)
		return (page);
	dirPTR = opendir(path);
	if (dirPTR == NULL)
	{
		std::cerr	<< "Couldn't open directory: "
					<< path
					<< std::endl;

		return (page);
	};
	pageAddOpening(page, requestREF.getUri());
	for (struct dirent *dirent = readdir(dirPTR); dirent != NULL; dirent = readdir(dirPTR))
	{
		page += direntLink(requestREF.getUri(), std::string(dirent->d_name), servConfREF.getListen().first, servConfREF.getListen().second);
	}
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

	if (stat(path.c_str(), &pathStat) < 0)
		return (-1);
	return (true);
};
