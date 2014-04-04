//============================================================================
// Name        : HW01.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include "XMLNode.h"
#include "XMLParser.h"
using namespace std;

#define MAX_CHAR_SIZE 100
#define MAX_BUF_SIZE 500

enum commandType { search_TagName, search_Attribute};

void searchAll(XMLNode* _XpathRoute, const char* str, commandType _commandType)
{
	list<XMLNode>::iterator _iter;
	list<tagAttribute>::iterator _iter2;

	if(_commandType == search_Attribute)
	{
		for(_iter2 = _XpathRoute->getAttribute()->begin(); _iter2 != _XpathRoute->getAttribute()->end(); _iter2++)
		{
			if(!strcmp(_iter2->getName(), str))
			{
				cout << _iter2->getName() << "\t" << _iter2->getValue() << endl;
			}
			else if(!strcmp(_iter2->getValue(), str))
			{
				cout << _iter2->getName() << "\t" << _iter2->getValue() << endl;
			}
		}
	}

	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
	{
		if(_commandType == search_TagName)
		{
			if(!strcmp(_iter->getTagName(), str))
			{
				cout << _iter->getTagName() << "\t" << _iter->getContent() << endl;
			}
		}
		searchAll(&(*_iter),str, _commandType);
	}
	return ;
}

int main()
{
	char* fileName	= 	new char[MAX_CHAR_SIZE];
	char* cmdBuf 	= 	new char[MAX_CHAR_SIZE];
	int cmdIdx		=	0;

	XMLParser XParser;
	XMLNode* XpathRoute = new XMLNode;
	list<XMLNode>::iterator iter;
	list<tagAttribute>::iterator iter2;

	while(1)
	{
		cout << "Á¾·á -> quit" << endl;
		cout << "Input FileName : ";
		cin >> fileName;
		if(!strcmp(fileName, "quit")) return 0;
		else if(!XParser.parser(fileName, XpathRoute)) break;
	}

	while(1)
	{
		cmdIdx = 0;
		cout << "cmd : ";
		cin >> cmdBuf;

		if(!strcmp(cmdBuf, "quit")) break;
		else
		{
			if(cmdBuf[cmdIdx] == '@')
			{
				searchAll(XpathRoute,&cmdBuf[cmdIdx+1], search_Attribute);
			}
			else if(cmdBuf[cmdIdx] == '/' && cmdBuf[cmdIdx+1] == '/')
			{
				searchAll(XpathRoute,&cmdBuf[cmdIdx+2], search_TagName);
			}
			else cout << cmdBuf << endl;
		}
	}

	delete[] fileName;
	delete[] cmdBuf;
	delete XpathRoute;

	/*
	cout << "press any key.." << endl;
	cin.ignore();
	cin.get();
	*/

	return 0;
}
