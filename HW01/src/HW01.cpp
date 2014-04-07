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
#include "XPath.h"
using namespace std;

#define MAX_CHAR_SIZE 500
#define MAX_BUF_SIZE 800

int main()
{
	char* fileName	= 	new char[MAX_CHAR_SIZE];
	char* cmdBuf 	= 	new char[MAX_CHAR_SIZE];
	int cmdIdx		=	0;

	XPath Xpath;
	XMLParser XParser;
	XMLNode* XpathRoute = new XMLNode;

	while(1)
	{
		cout << "Input FileName(quit) : ";
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
				Xpath.Search_All(XpathRoute,&cmdBuf[cmdIdx+1], search_Attribute);
			}
			else if(cmdBuf[cmdIdx] == '/' && cmdBuf[cmdIdx+1] == '/')
			{
				Xpath.Search_All(XpathRoute,&cmdBuf[cmdIdx+2], search_TagName);
			}
			else cout << cmdBuf << endl;
		}
	}

	delete[] fileName;
	delete[] cmdBuf;
	delete XpathRoute;

	return 0;
}
