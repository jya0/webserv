/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/14 18:49:22 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"
#include 	<utility>

static ADirective	*tokenToDirective(std::vector<std::string> &tokens)
{
	DirectiveSimple	*directive;

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

	do {
		befParesableLine = configIF.tellg();
	} while (getline(configIF, tmpLine) && tmpLine.find_first_not_of(SPACE_CHARSET) == std::string::npos);
	lineREF = tmpLine;
	return (befParesableLine);
}

static std::vector<std::string>	tokenize(const std::string &lineREF)
{
	std::vector<std::string>	tmpTokens;
	std::size_t					start;
	std::size_t					end;

	start = 0;
	while (start < lineREF.length())
	{
		end = lineREF.find_first_of(SPACE_CHARSET, start);
		if (end == std::string::npos && start < lineREF.length())
		{
			tmpTokens.push_back(lineREF.substr(start, end));
			break ;
		}
		if (end > start)
			tmpTokens.push_back(lineREF.substr(start, end - start));
		start = end + 1;
	}
	return (tmpTokens);
}

static DirectiveBlock	*initDirectiveBlock(std::ifstream &configIF, int &tabBlockCountREF)
{
	std::string						line;
	std::vector<std::string>		tokens;
	DirectiveBlock					*dveBlock;
	ADirective						*dveBlockInfo;

	getParesableLine(configIF, line);
	tokens = tokenize(line);
	dveBlockInfo = tokenToDirective(tokens);
	dveBlock = new DirectiveBlock();
	*((ADirective *)(dveBlock)) = *(dveBlockInfo);
	delete (dveBlockInfo);
	tabBlockCountREF = countLeadingTabs(line);
	return (dveBlock);
}

static DirectiveBlock	*getNextDirectiveBlock(std::ifstream &configIF)
{
	int								tabBlockCount;
	DirectiveBlock					*dveBlock;
	std::string						lineCnt;
	std::string						lineNxt;
	std::streampos					befCnt;
	std::streampos					befNxt;
	std::vector<std::string>		tokens;
	ADirective						*dveToInsert;

	dveBlock = initDirectiveBlock(configIF, tabBlockCount);
	befCnt = getParesableLine(configIF, lineCnt);
	while (!lineCnt.empty() && countLeadingTabs(lineCnt) > tabBlockCount)
	{
		dveToInsert = NULL;
		befNxt = getParesableLine(configIF, lineNxt);
		if (countLeadingTabs(lineCnt) == tabBlockCount + 1 && countLeadingTabs(lineNxt) <= tabBlockCount + 1)
		{
			tokens = tokenize(lineCnt);
			dveToInsert = tokenToDirective(tokens);
			befCnt = befNxt;
			lineCnt = lineNxt;
		}
		else if (countLeadingTabs(lineCnt) == tabBlockCount + 1 && countLeadingTabs(lineNxt) == tabBlockCount + 2)
		{
			configIF.seekg(befCnt);
			dveToInsert = getNextDirectiveBlock(configIF);
			befCnt = getParesableLine(configIF, lineCnt);
			befNxt = befCnt;
		}
		else
		{
			std::cerr << "Error tab level at directive/ directive block: " << lineCnt << std::endl;
			delete (dveBlock);
			return (NULL);
		}
		dveBlock->insertMapDirective(dveToInsert);
	}
	configIF.seekg(befNxt);
	return (dveBlock);
}

const std::vector<std::string> HTTPServerParser::dveNames = tokenize(SIMPLE_DIRECTIVES BLOCK_DIRECTIVES);

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
			serverBlock->parseDirective();
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
