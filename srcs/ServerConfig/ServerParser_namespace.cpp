/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/12/05 16:12:12 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"ServerParser_namespace.hpp"
#include	"ServerConfig.hpp"
#include	"Autoindex_namespace.hpp"
#include 	<utility>
#include	<fstream>
#include	<sstream>
#include	<cstdlib>

using namespace http;

const std::vector< std::string >	ServerParser::dveNames = ServerParser::tokenize(SIMPLE_DIRECTIVES BLOCK_DIRECTIVES);

std::pair< std::string, std::string >	ServerParser::splitByTwo(const std::string &strREF, const char &delimREF)
{
	std::stringstream	ss;
	std::string			first;
	std::string			second;

	ss.str(strREF);
	std::getline(ss, first, delimREF);
	ss >> second;
	return (std::make_pair(first, second));
}

/**
 * @brief To get any uri to the form /uri/
 * `
 * @param uri
 * @return std::string
 */
std::string	ServerParser::appendSlashes(std::string uri)
{
	if (*(uri.begin()) != '.' && uri.size() > 1)
	{
		if (*(uri.begin()) != '/')
			uri = "/" + uri;
		if (*(uri.rbegin()) != '/')
			uri += "/";
	}
	return (uri);
}

static ADirective	*tokenToDirective(std::vector< std::string > &tokens) {
	DirectiveSimple	*directive;

	directive = new DirectiveSimple();
	directive->setName(tokens.front());
	tokens.erase(tokens.begin());
	directive->setValues(tokens);
	tokens.clear();
	return (directive);
}

static int	countLeadingTabs(std::string lineCnt) {
	std::size_t	tabCount;

	tabCount = lineCnt.find_first_not_of('\t');
	if (tabCount != std::string::npos)
		return (tabCount);
	return (0);
}

static std::streampos	getParesableLine(\
std::ifstream &configIF, std::string &lineREF) {
	std::string			tmpLine;
	std::streampos		befParesableLine;

	do {
		befParesableLine = configIF.tellg();
	} while (getline(configIF, tmpLine) && tmpLine.find_first_not_of(SPACE_CHARSET) == std::string::npos);
	lineREF = tmpLine;
	return (befParesableLine);
}

std::vector< std::string >	ServerParser::tokenize(const std::string &lineREF) {
	std::vector< std::string >	tmpTokens;
	std::istringstream			stringStream;
	std::string					token;

	stringStream.str(lineREF);
	while (stringStream)
	{
		stringStream >> token;
		if (stringStream)
			tmpTokens.push_back(token);
	}
	return (tmpTokens);
}

static DirectiveBlock	*initDirectiveBlock(std::ifstream &configIF, int &tabBlockCountREF) {
	std::string						line;
	std::vector< std::string >		tokens;
	DirectiveBlock					*dveBlock;
	ADirective						*dveBlockInfo;

	getParesableLine(configIF, line);
	tokens = ServerParser::tokenize(line);
	dveBlockInfo = tokenToDirective(tokens);
	dveBlock = new DirectiveBlock();
	*((ADirective *)(dveBlock)) = *(dveBlockInfo);
	delete (dveBlockInfo);
	tabBlockCountREF = countLeadingTabs(line);
	return (dveBlock);
}

static DirectiveBlock	*getNextDirectiveBlock(std::ifstream &configIF) {
	int								tabBlockCount;
	DirectiveBlock					*dveBlock;
	std::string						lineCnt;
	std::string						lineNxt;
	std::streampos					befCnt;
	std::streampos					befNxt;
	std::vector< std::string >		tokens;
	ADirective						*dveToInsert;

	dveBlock = initDirectiveBlock(configIF, tabBlockCount);
	befCnt = getParesableLine(configIF, lineCnt);
	while (!lineCnt.empty() && countLeadingTabs(lineCnt) > tabBlockCount)
	{
		dveToInsert = NULL;
		befNxt = getParesableLine(configIF, lineNxt);
		if (countLeadingTabs(lineCnt) == tabBlockCount + 1 && countLeadingTabs(lineNxt) <= tabBlockCount + 1)
		{
			tokens = ServerParser::tokenize(lineCnt);
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
			std::cerr << "Error tab level at directive/ directive block: " << "\""  << lineCnt << "\"" << std::endl;
			delete (dveBlock);
			return (NULL);
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

std::vector< ServerConfig >	ServerParser::parseConfigFile(const std::string &filename) {
	DirectiveBlock					*dveBlock;
	std::vector< ServerConfig >	serverBlocks;
	std::ifstream					configIF;

	dveBlock = NULL;
	if (Autoindex::isPathFolder(filename) > 0)
		throw (ServerParser::ParseErrorException("Invalid file provided!"));
	configIF.open(filename.c_str(), std::ios::in);
	if (!configIF.is_open())
		throw (ServerParser::ParseErrorException("Could not open config file!"));
	try {
		while (configIF.peek() != EOF)
		{
			dveBlock = getNextDirectiveBlock(configIF);
			if (dveBlock == NULL)
				throw (ParseErrorException("NULL directive block!"));
			else
			{
				dveBlock->printDirective();
				dveBlock->parseDirective();
				dveBlock->checkDirective(HTTP);
				serverBlocks.push_back(ServerConfig(dveBlock));
				delete (dveBlock);
			}
		}
		configIF.close();
	}
	catch (std::exception &e)
	{
		std::cerr	<< e.what()
					<< std::endl;
		configIF.close();
		delete (dveBlock);
		serverBlocks.clear();
		throw (ParseErrorException("Parser Error!"));
	}
	return (serverBlocks);
}
