/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 14:44:39 by jyao              #+#    #+#             */
/*   Updated: 2023/09/03 12:47:05 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"HTTPServerParser.hpp"

static ADirective	*getDirective(std::stack<std::string> &tmpStack)
{
	DirectiveSimple				*directive;
	std::string					dveName;
	std::vector<std::string>	dveValues;

	directive = new DirectiveSimple();
	// dveValues = ;
	directive->setValues(dveValues);
	// dveName = ;
	directive->setName(dveName);
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
	return ("");
}

static void	pushLineToStack(std::stack<std::string> &tmpStack, std::string line)
{
}

static DirectiveBlock	*getNextDirectiveBlock(std::ifstream &configIF)
{
	std::stack<std::string>			tmpStack;
	int								blockTabCount;
	std::string						tmpLine;
	std::string						nxtLine;
	std::streampos					oldpos;
	DirectiveBlock					*dveBlock;
	ADirective						*dveToInsert;

	dveBlock = new DirectiveBlock();
	tmpLine = getReadableLine(configIF);
	pushLineToStack(tmpStack, tmpLine);
	(ADirective)(*dveBlock) = (*getDirective(tmpStack));
	blockTabCount = countLeadingTabs(tmpLine);
	tmpLine = getReadableLine(configIF);
	oldpos = configIF.tellg();
	nxtLine = getReadableLine(configIF);
	while (countLeadingTabs(tmpLine) != blockTabCount)
	{
		if (countLeadingTabs(tmpLine) == blockTabCount + 1 && countLeadingTabs(nxtLine) == blockTabCount + 1)
		{
			pushLineToStack(tmpStack, tmpLine);
			dveToInsert = getDirective(tmpStack);
		}
		else if (countLeadingTabs(nxtLine) > blockTabCount + 1)
		{
			configIF.seekg(oldpos);
			dveToInsert = getNextDirectiveBlock(configIF);
		}
		dveBlock->getDirectives()->insert(std::pair< std::string, ADirective * >(dveToInsert->getName(), dveToInsert));
		tmpLine = getReadableLine(configIF);
		oldpos = configIF.tellg();
		nxtLine = getReadableLine(configIF);
	}
	return (dveBlock);
}

std::vector<WebServer> HTTPServerParser::parseConfigFile(std::string filename)
{
    //code to parse webserv config file goes here
	std::vector<WebServer>	servers;
	DirectiveBlock			*serverDveBlock;
	std::ifstream			configIF;
	WebServer				*server;
	configIF.open(filename, std::ios::in);
	if (!configIF.is_open())
		// exit(1);
	while (!configIF.eof())
	{
		// getNextDirectiveBlock(configIF);
		if (serverDveBlock == NULL)
			//exit(1);
		server = new WebServer(*serverDveBlock);
		servers.push_back(*server);
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