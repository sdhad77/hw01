/*
 * XPath.cpp
 *
 *  Created on: 2014. 4. 7.
 *      Author: 신동환
 */

#include "XPath.h"

XPath::XPath() {
}

XPath::~XPath() {
}

int XPath::XPathCmdParser(char* _cmdBuf, XMLNode* _XpathRoute)
{
	int _cmdIdx = 0;
	if(_cmdBuf[_cmdIdx] == '@') std::cout << "@입력" << std::endl;
}

void XPath::Search_All(XMLNode* _XpathRoute, const char* str, commandType _commandType)
{
	std::list<XMLNode>::iterator _iter;
	std::list<tagAttribute>::iterator _iter2;

	if(_commandType == search_TagName)
	{
		if(!strcmp(_XpathRoute->getTagName(), str)) AddNodeList(_XpathRoute);
	}

	else if(_commandType == search_Attribute)
	{
		for(_iter2 = _XpathRoute->getAttribute()->begin(); _iter2 != _XpathRoute->getAttribute()->end(); _iter2++)
		{
			if(!strcmp(_iter2->getName(), str))			std::cout << _iter2->getName() << "\t" << _iter2->getValue() << std::endl;
			else if(!strcmp(_iter2->getValue(), str))	std::cout << _iter2->getName() << "\t" << _iter2->getValue() << std::endl;
		}
	}

	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
		Search_All(&(*_iter),str, _commandType);
}

void XPath::AddNodeList(XMLNode* node)
{
	nodeList.push_back(node);
}

void XPath::ClearNodeList()
{
	nodeList.clear();
}

void XPath::PrintNodeList()
{
	std::list<XMLNode *>::iterator _iter;
	for(_iter = nodeList.begin(); _iter != nodeList.end(); _iter++)
	{
		(*_iter)->PrintNode();
	}
}
