/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/09/03 14:00:22 by jyao             ###   ########.fr       */
=======
/*   Updated: 2023/09/03 12:47:05 by rriyas           ###   ########.fr       */
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"

static ADirective	*getDirective(std::vector<std::string> &tokens)
{
	DirectiveSimple				*directive;

	if (tokens.front().find_first_of(":", 0) == std::string::npos)
		return (NULL);
	directive = new DirectiveSimple();
<<<<<<< HEAD
	directive->setName(tokens.front());
	tokens.erase(tokens.begin());
	directive->setValues(tokens);
=======
	// dveValues = ;
	directive->setValues(dveValues);
	// dveName = ;
	directive->setName(dveName);
	return (directive);
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
}

static int	countLeadingTabs(std::string cntline)
{
	std::size_t	tabCount;

	tabCount = cntline.find_first_not_of('\t');
	if (tabCount != std::string::npos)
		return (tabCount);
	return (0);
}

static std::string	getReadableLine(std::ifstream &configIF)
{
<<<<<<< HEAD
	std::string	tmpLine;

	getline(configIF, tmpLine);
	while (tmpLine.length() == 0)
		getline(configIF, tmpLine);
	return (tmpLine);
=======
	return ("");
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
}

static void	tokenize(std::vector<std::string> &tokens, std::string line)
{
	std::size_t	start;
	std::size_t	end;

	start = 0;
	while (start < line.length())
	{
		end = line.find_first_of(SPACE_CHARSET, start);
		if (end == std::string::npos && start < line.length())
		{
			tokens.push_back(line.substr(start, end));
			return ;
		}
		if (end > start)
			tokens.push_back(line.substr(start, end - start));
		start = end + 1;
	}
}

static DirectiveBlock	*getNextDirectiveBlock(std::ifstream &configIF)
{
	std::vector<std::string>		tokens;
	int								blockTabCount;
	std::string						cntLine;
	std::string						nxtLine;
	std::streampos					oldpos;
	DirectiveBlock					*blockDve;
	ADirective						*dveToInsert;

	blockDve = new DirectiveBlock();
	cntLine = getReadableLine(configIF);
	tokenize(tokens, cntLine);
	(ADirective)(*blockDve) = (*getDirective(tokens));
	blockTabCount = countLeadingTabs(cntLine);
	cntLine = getReadableLine(configIF);
	oldpos = configIF.tellg();
	nxtLine = getReadableLine(configIF);
	while (countLeadingTabs(cntLine) != blockTabCount)
	{
		if (countLeadingTabs(cntLine) == blockTabCount + 1 && countLeadingTabs(nxtLine) == blockTabCount + 1)
		{
			tokenize(tokens, cntLine);
			dveToInsert = getDirective(tokens);
		}
		else if (countLeadingTabs(nxtLine) > blockTabCount + 1)
		{
			configIF.seekg(oldpos);
			dveToInsert = getNextDirectiveBlock(configIF);
		}
		if (dveToInsert == NULL)
			// exit(1);
		blockDve->getDirectives()->insert(std::pair< std::string, ADirective * >(dveToInsert->getName(), dveToInsert));
		cntLine = getReadableLine(configIF);
		oldpos = configIF.tellg();
		nxtLine = getReadableLine(configIF);
	}
	return (blockDve);
}

std::vector<WebServer> HTTPServerParser::parseConfigFile(std::string filename)
{
    //code to parse webserv config file goes here
	std::vector<WebServer>	servers;
	DirectiveBlock			*serverBlock;
	std::ifstream			configIF;
	WebServer				*server;
	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
		// exit(1);
	while (!configIF.eof())
	{
<<<<<<< HEAD
		getNextDirectiveBlock(configIF);
		if (serverBlock == NULL)
			//exit(1);
		servers.push_back(WebServer(serverBlock));
=======
		// getNextDirectiveBlock(configIF);
		if (serverDveBlock == NULL)
			//exit(1);
		server = new WebServer(*serverDveBlock);
		servers.push_back(*server);
>>>>>>> 28973d771451fe849b0f51c896c2493ec9e23437
	}
	configIF.close();
	return (servers);
}

const char *HTTPServerParser::ParseError::what() const throw()
{
    return ("Error while parsing file!\n");
}

HTTPServerParser::HTTPServerParser()
{

}

HTTPServerParser::~HTTPServerParser()
{

}