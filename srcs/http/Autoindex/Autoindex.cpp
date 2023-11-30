/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:09:04 by jyao              #+#    #+#             */
/*   Updated: 2023/11/27 16:57:49 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<dirent.h>
#include	<iostream>
#include	<sstream>
#include	"Autoindex_namespace.hpp"

static std::string	direntLink(const std::string &dirName, const std::string &dirent, const std::string &hostREF, const int &portREF)
{
	std::stringstream	ssDirentLink;

	ssDirentLink <<	"\t\t<p><a href=\"http://" + hostREF + ":" << portREF << dirName + "/" + dirent + "\">" + dirent + "</a></p>\n";
	return (ssDirentLink.str());
};

static void	pageAddOpening(std::string &pageREF, const std::string &dirNameREF)
{
	pageREF +=	"<!DOCTYPE html>\n"
					"<html>\n"
						"<head>\n"
	        				"<title>" + dirNameREF + "</title>\n"
						"</head>\n"
						"<body>\n"
							"<h1>INDEX</h1>\n"
							"<p>\n";
};

static void	pageAddClosing(std::string &pageREF)
{
	pageREF +=	"</p>\n"
				"</body>\n"
    			"</html>\n";
};

std::string	Autoindex::genPage(const char *path, const std::string &hostREF, const int &portREF)
{
	DIR			*dirPTR;
	std::string	dirNameSTR;
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
	dirNameSTR = path;
	if (dirNameSTR.front() != '/')
		dirNameSTR = '/' + dirNameSTR;
	pageAddOpening(page, dirNameSTR);
	for (struct dirent *dirent = readdir(dirPTR); dirent != NULL; dirent = readdir(dirPTR))
	{
		page += direntLink(dirNameSTR, std::string(dirent->d_name), hostREF, portREF);
	}
	pageAddClosing(page);
	return (page);
};
