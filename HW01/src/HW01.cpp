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

void printAll(XMLNode* _XpathRoute)
{
	list<XMLNode>::iterator _iter;
	cout << _XpathRoute->getTagName() << endl;
	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
	{
		printAll(&(*_iter));
	}
	return ;
}

void searchAll(XMLNode* _XpathRoute, const char* str)
{
	list<XMLNode>::iterator _iter;
	if(!strcmp(_XpathRoute->getTagName(), str))
	{
		cout << _XpathRoute->getTagName() << "\t" << _XpathRoute->getContent() << endl;
	}

	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
	{
		searchAll(&(*_iter),str);
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
			searchAll(XpathRoute,cmdBuf);
			/*
			while(cmdBuf[cmdIdx] != '\0')
			{
				if(cmdBuf[cmdIdx] == '/')
				{
					if(cmdBuf[cmdIdx+1] == '/')
					{
						while(cmdBuf[cmdIdx+2] != '\0' && cmdBuf[cmdIdx+2] != '/')
						{
							cout << cmdBuf[cmdIdx+2];
							cmdIdx++;
						}
						cout << endl;
						cmdIdx += 1;
					}
				}
				else cout << cmdBuf[cmdIdx] << endl;
				cmdIdx++;
			}*/
		}
/*		else if(!strcmp(cmdBuf, "up")) XpathRoute = XpathRoute->getParentNode();
		else if(!strcmp(cmdBuf, "down"))
		{
			cout << "child name : ";
			cin >> cmdBuf;

			bool searchComplete = false;
			for(iter = XpathRoute->getChildNode()->begin(); iter != XpathRoute->getChildNode()->end(); iter++)
			{
				if(!strcmp(cmdBuf, iter->getTagName()))
				{
					searchComplete = true;
					XpathRoute = &(*iter);
					break;
				}
			}
			if(!searchComplete) cout << "fail" << endl;
		}
		else if(!strcmp(cmdBuf, "print"))
		{
			cout << "tagName : " << XpathRoute->getTagName() << endl;
			cout << "content : " << XpathRoute->getContent() << endl;
			cout << "childNodeSize : " << XpathRoute->getChildNode()->size() << endl;
			if(XpathRoute->getChildNode()->size())
			{
				cout << "ChildTagName : ";
				for(iter = XpathRoute->getChildNode()->begin(); iter != XpathRoute->getChildNode()->end(); iter++)
				{
					cout << &(*iter->getTagName()) << " ";
				}
				cout << endl;
			}
			cout << "AttributeSize : " << XpathRoute->getAttribute()->size() << endl;
			if(XpathRoute->getAttribute()->size())
			{
				for(iter2 = XpathRoute->getAttribute()->begin(); iter2 != XpathRoute->getAttribute()->end(); iter2++)
				{
					cout << "AttributeName : " << &(*iter2->getName()) << endl;
					cout << "AttributeValue : " << &(*iter2->getValue()) << endl;
				}
			}
		}
*/
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
