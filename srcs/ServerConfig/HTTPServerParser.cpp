/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/08 10:51:42 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"
#include 	<utility>

static ADirective	*getDirective(std::vector<std::string> &tokens)
{
	DirectiveSimple				*directive;
	std::size_t					colonLoc;

	colonLoc = tokens.front().find_first_of(":", 0);
	if (colonLoc == std::string::npos)
	{
		std::cerr << "Error ':' at " << tokens.front() << std::endl;
		return (NULL);
	}
	directive = new DirectiveSimple();
	directive->setName(tokens.front());
	tokens.erase(tokens.begin());
	directive->setValues(tokens);
	tokens.clear();
	return (directive);
}

static int	countLeadingTabs(std::string lineCnt)
{
	std::size_t	tabCount;

	tabCount = lineCnt.find_first_not_of('\t');
	if (tabCount != std::string::npos)
		return (tabCount);
	return (0);
}

static std::streampos	getParesableLine(\
std::ifstream &configIF, std::string &lineREF)
{
	std::string	tmpLine;
	std::streampos	befParesableLine;

	befParesableLine = configIF.tellg();
	getline(configIF, tmpLine);
	while (!tmpLine.empty() && tmpLine.find_first_not_of(SPACE_CHARSET) == std::string::npos)
	{
		befParesableLine = configIF.tellg();
		getline(configIF, tmpLine);
	}
	lineREF = tmpLine;
	return (befParesableLine);
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

static DirectiveBlock	*initDveBlock(std::ifstream &configIF, int &tabBlockREF)
{
	std::string						line;
	std::vector<std::string>		tokens;
	DirectiveBlock					*dveBlock;
	ADirective						*dveBlockInfo;

	getParesableLine(configIF, line);
	tokenize(tokens, line);
	dveBlockInfo = getDirective(tokens);
	if (dveBlockInfo == NULL)
		return (NULL);
	dveBlock = new DirectiveBlock();
	*((ADirective *)(dveBlock)) = *(dveBlockInfo);
	delete (dveBlockInfo);
	tabBlockREF = countLeadingTabs(line);
	return (dveBlock);
}

static DirectiveBlock	*getNextDirectiveBlock(std::ifstream &configIF)
{
	int								tabBlock;
	DirectiveBlock					*dveBlock;
	std::string						lineCnt;
	std::string						lineNxt;
	std::streampos					befCnt;
	std::streampos					befNxt;
	std::vector<std::string>		tokens;
	ADirective						*dveToInsert;

	dveBlock = initDveBlock(configIF, tabBlock);
	if (dveBlock == NULL)
		return (NULL);
	befCnt = getParesableLine(configIF, lineCnt);
	while (!lineCnt.empty() && countLeadingTabs(lineCnt) != tabBlock)
	{
		dveToInsert = NULL;
		befNxt = getParesableLine(configIF, lineNxt);
		if (countLeadingTabs(lineCnt) == tabBlock + 1 && countLeadingTabs(lineNxt) <= tabBlock + 1)
		{
			tokenize(tokens, lineCnt);
			dveToInsert = getDirective(tokens);
			befCnt = befNxt;
			lineCnt = lineNxt;
		}
		else if (countLeadingTabs(lineNxt) > tabBlock + 1)
		{
			configIF.seekg(befCnt);
			dveToInsert = getNextDirectiveBlock(configIF);
			befCnt = getParesableLine(configIF, lineCnt);
		}
		if (dveToInsert == NULL)
		{
			delete (dveBlock);
			std::cerr << "Error at line: " << lineCnt << std::endl;
			return (NULL);
		}
		dveBlock->insertMapDirective(dveToInsert);
	}
	configIF.seekg(befNxt);
	return (dveBlock);
}

static void	printServerBlock(DirectiveBlock	*serverBlock)
{
	
}

void	HTTPServerParser::parseConfigFile1(std::string filename)
{
	DirectiveBlock			*serverBlock;
	std::ifstream			configIF;

	serverBlock = NULL;
	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		// return (NULL);
	}
	if (configIF.is_open())
		serverBlock = getNextDirectiveBlock(configIF);
	while (!configIF.eof())
	{
		serverBlock = getNextDirectiveBlock(configIF);
		printServerBlock(serverBlock);
		delete (serverBlock);
	}
	configIF.close();
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