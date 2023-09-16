/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/16 15:50:11 by jyao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"
#include 	<utility>

# define	SPACE_CHARSET	" \f\n\r\t\v"

/**
 * @brief A static function which takes the tokens and fills it
 * into a simple directive with only ADirective::_dveName 
 * and ADirective::_dveValues filled
 * its ADirective::_dveType will be filled later in 
 * ADirective::parserDirective(void)
 * 
 * @param tokens vector of strings gotten from tokenize(const std::string &lineREF)
 * @return ADirective* 
 */
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

/**
 * @brief simply counts how many leading tab character there are 
 * before the first non white space character, this will be used as that
 * block directive's tab level
 * 
 * @param lineCnt the line read from std::getline()
 * @return int
 * returns 0 by default because it means there are no leading tab characters
 */
static int	countLeadingTabs(std::string lineCnt)
{
	std::size_t	tabCount;

	tabCount = lineCnt.find_first_not_of('\t');
	if (tabCount != std::string::npos)
		return (tabCount);
	return (0);
}

/**
 * @brief loop calls std::getline() until a line that's not empty
 * or is filled entirely with white-spaces
 * 
 * @param configIF is the infile stream object used by std::getline()
 * @param lineREF is the string reference used to store the values outside
 * of this function's scope
 * 
 * @return std::streampos
 * returns the saved stream position before 
 * the point when it gets a parseable line.
 * when this streampos is used it can get the same line again.
 * basically it allows us to peek at the next line without changing
 * the stream buffer position
 */
static std::streampos	getParesableLine(std::ifstream &configIF, std::string &lineREF)
{
	std::string			tmpLine;
	std::streampos		befParesableLine;

	do {
		befParesableLine = configIF.tellg();
	} while (getline(configIF, tmpLine) && tmpLine.find_first_not_of(SPACE_CHARSET) == std::string::npos);
	lineREF = tmpLine;
	return (befParesableLine);
}

/**
 * @brief uses istringstream to get separate words delimited
 * by white-spaces in a sentence
 * 
 * @param lineREF the line you want to split up in to a vector of strings
 * @return std::vector<std::string> 
 */
static std::vector<std::string>	tokenize(const std::string &lineREF)
{
	std::vector<std::string>	tmpTokens;
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

/**
 * @brief creates a new DirectiveBlock object for the return 
 * of the first getNextDirectiveBlock(std::ifstream &configIF) call
 * this will return a DirectiveBlock of the server directive
 * 
 * @param configIF is the infile stream object used by std::getline()
 * @param tabBlockCountREF used to store block directive's tab level
 * outside of the scope of this function.
 * 
 * @return DirectiveBlock* 
 */
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

/**
 * @brief a recursive function which will load everything into a multimap
 * contained by a DirectiveBlock
 * 
 * The meat of the recursion has 2 parts.
 * part 1 will just retrieve single line simple directive
 * 
 * part 2 will call recursively to retrieve a multi-line block directive
 * 
 * @param configIF is the infile stream object used by std::getline()
 * @return DirectiveBlock* 
 */
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

/**
 * @brief initialise dveNames which is used later for ADirective::parseDirective(void) to
 * assign the correct ADirective::_dveType
 * 
 */
const std::vector<std::string> HTTPServerParser::dveNames = tokenize(SIMPLE_DIRECTIVES BLOCK_DIRECTIVES);

/**
 * @brief used to return a vector of server block directives
 * 
 * @param filename the path to the webserver config file
 * @return std::vector< DirectiveBlock * > 
 */
std::vector< DirectiveBlock * >	HTTPServerParser::parseConfigFile(std::string filename)
{
	DirectiveBlock					*dveBlock;
	std::vector< DirectiveBlock * >	serverBlocks;
	std::ifstream					configIF;

	dveBlock = NULL;
	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		exit(1);
	}
	while (configIF.peek() != EOF)
	{
		dveBlock = getNextDirectiveBlock(configIF);
		if (dveBlock == NULL)
		{
			serverBlocks.clear();
			break ;
		}
		else
		{
			dveBlock->printDirective();
			dveBlock->parseDirective();
			serverBlocks.push_back(dveBlock);
		}
	}
	configIF.close();
	return (serverBlocks);
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
