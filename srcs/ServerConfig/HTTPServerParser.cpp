/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/12 10:22:53 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"
#include 	<utility>


static ADirective	*getDirective(std::vector<std::string> &tokens)
{
	DirectiveSimple				*directive;

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
	std::string			tmpLine;
	std::streampos		befParesableLine;
	int					calls;

	calls = 0;
	do {
		calls++;
		befParesableLine = configIF.tellg();
	} while (getline(configIF, tmpLine) && tmpLine.find_first_not_of(SPACE_CHARSET) == std::string::npos);
	lineREF = tmpLine;
	return (befParesableLine);
}

static int	tokenize(std::vector<std::string> &tokens, std::string &lineREF)
{
	static int	nonSpaceLineNo;
	std::size_t	start;
	std::size_t	end;
	std::size_t	colonLoc;

	++nonSpaceLineNo;
	start = 0;
	while (start < lineREF.length())
	{
		end = lineREF.find_first_of(SPACE_CHARSET, start);
		if (end == std::string::npos && start < lineREF.length())
		{
			tokens.push_back(lineREF.substr(start, end));
			break ;
		}
		if (end > start)
			tokens.push_back(lineREF.substr(start, end - start));
		start = end + 1;
	}
	colonLoc = tokens.front().find_first_of(":", 0);
	if (colonLoc == std::string::npos)
	{
		std::cerr << "Error ':' at " << tokens.front() << std::endl;
		std::cerr << "Error at non-space line " << nonSpaceLineNo << ": \"" << lineREF << "\"" << std::endl;
		return (-1);
	}
	return (0);
}

static DirectiveBlock	*initDveBlock(std::ifstream &configIF, int &tabBlockREF)
{
	std::string						line;
	std::vector<std::string>		tokens;
	DirectiveBlock					*dveBlock;
	ADirective						*dveBlockInfo;

	getParesableLine(configIF, line);
	if (tokenize(tokens, line))
		return (NULL);
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
			if (tokenize(tokens, lineCnt) == 0)
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
			return (NULL);
		}
		dveBlock->insertMapDirective(dveToInsert);
	}
	configIF.seekg(befNxt);
	return (dveBlock);
}

// std::vector<WebServer *>	HTTPServerParser::parseConfigFile(std::string filename)
void	HTTPServerParser::parseConfigFile(std::string filename)
{
	DirectiveBlock				*serverBlock;
	// std::vector<WebServer *>	servers;
	std::ifstream				configIF;

	serverBlock = NULL;
	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		exit(1);
	}
	while (configIF.peek() != EOF)
	{
		serverBlock = getNextDirectiveBlock(configIF);
		if (serverBlock == NULL)
		{
			// servers.clear();
			break ;
		}
		else
		{
			serverBlock->printDirective();
			// servers.push_back(new WebServer(*serverBlock));
			delete (serverBlock);
		}
	}
	configIF.close();
	// return (servers);
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