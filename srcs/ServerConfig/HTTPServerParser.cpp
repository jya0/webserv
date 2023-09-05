/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 18:42:49 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"
#include 	<utility>
static ADirective	*getDirective(std::vector<std::string> &tokens)
{
	DirectiveSimple				*directive;

	if (tokens.front().find_first_of(":", 0) == std::string::npos)
		return (NULL);
	directive = new DirectiveSimple();
	directive->setName(tokens.front());
	tokens.erase(tokens.begin());
	directive->setValues(tokens);
	tokens.clear();
	return (directive);
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
	std::string	tmpLine;

	getline(configIF, tmpLine);
	while (tmpLine.length() == 0 && !configIF.eof())
		getline(configIF, tmpLine);
	return (tmpLine);
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
	*((ADirective *)(blockDve)) = *(getDirective(tokens));
	blockTabCount = countLeadingTabs(cntLine);
	cntLine = getReadableLine(configIF);
	oldpos = configIF.tellg();
	nxtLine = getReadableLine(configIF);
	while (cntLine.length() > 0 && countLeadingTabs(cntLine) != blockTabCount)
	{
		if (countLeadingTabs(cntLine) == blockTabCount + 1 && countLeadingTabs(nxtLine) <= blockTabCount + 1)
		{
			tokenize(tokens, cntLine);
			dveToInsert = getDirective(tokens);
			cntLine = nxtLine;
			nxtLine = getReadableLine(configIF);
			if (countLeadingTabs(nxtLine) == blockTabCount + 1)
				oldpos = configIF.tellg();
		}
		else if (countLeadingTabs(nxtLine) > blockTabCount + 1)
		{
			configIF.seekg(oldpos);
			dveToInsert = getNextDirectiveBlock(configIF);
			cntLine = getReadableLine(configIF);
			oldpos = configIF.tellg();
			nxtLine = getReadableLine(configIF);
		}
		// if (dveToInsert == NULL)
			// exit(1);
		blockDve->insertMapDirective(dveToInsert);
	}
	return (blockDve);
}

DirectiveBlock	*HTTPServerParser::parseConfigFile1(std::string filename)
{
	DirectiveBlock			*serverBlock;
	std::ifstream			configIF;

	serverBlock = NULL;
	configIF.open(filename, std::ios::in);
	if (configIF.is_open())
		serverBlock = getNextDirectiveBlock(configIF);
		// exit(1);
	configIF.close();
	return (serverBlock);
}

/* std::vector<WebServer> HTTPServerParser::parseConfigFile(std::string filename)
{
    //code to parse webserv config file goes here
	std::vector<WebServer>	servers;
	DirectiveBlock			*serverBlock;
	std::ifstream			configIF;

	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
		// exit(1);
	while (!configIF.eof())
	{
		serverBlock = getNextDirectiveBlock(configIF);
		// if (serverBlock == NULL)
			//exit(1);
		// servers.push_back(WebServer(*serverBlock));
	}
	configIF.close();
	return (servers);
} */

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