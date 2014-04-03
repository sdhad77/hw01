//============================================================================
// Name        : HW01.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <fstream>
#include "XMLNode.h"
#include "XMLParser.h"
using namespace std;

#define MAX_CHAR_SIZE 100
#define MAX_BUF_SIZE 500


bool checkAlpha(const char ch)
{
	if ('a' <= ch && ch <= 'z') return true;
	else if ('A' <= ch && ch <= 'Z') return true;

	return false;
}

bool checkNumber(const char ch)
{
	if ('0' <= ch && ch <= '9') return true;

	return false;
}

int checkBlank(const char* str)
{
	int idx = 0;
	while (str[idx] != '\0')
	{
		if (str[idx] == ' ') return idx;

		idx++;
	}
	return idx;
}

int checkStartPoint(const char* str)
{
	int idx = 0;
	while (str[idx] != '<') idx++;

	return idx;
}

int checkEndPoint(const char* str)
{
	int idx = 0;
	while (str[idx] != '>') idx++;

	return idx;
}

int main()
{
	char* fileName	= 	new char[MAX_CHAR_SIZE];
	char* tempElement =	new char[MAX_CHAR_SIZE];
	char* cmdBuf 	= 	new char[MAX_CHAR_SIZE];
	char* buf 		= 	new char[MAX_BUF_SIZE];
	char* tempBuf 	= 	new char[MAX_BUF_SIZE];

	int idx = 0;
	int startIdx = 0;
	int endIdx = 0;
	bool isRoute = true;
	bool isEmptyTag = false;

	XMLNode* XpathRoute = new XMLNode;
	list<XMLNode>::iterator iter;

	cout << "Input FileName : ";
	cin >> fileName;

	ifstream fin(fileName);
	if(!fin)
	{
		cout << "error1" << endl;
		return 1;
	}

	while(fin.getline(buf, MAX_BUF_SIZE))
	{
		idx = 0;
		while(buf[idx] != '\0')
		{
			while(buf[idx] == ' ') idx++; // 공백문자 제거

			if(buf[idx] == '<')
			{
				idx++;
				startIdx = idx;
				endIdx = checkEndPoint(&buf[startIdx]);
				strncpy(tempBuf, &buf[startIdx], endIdx);
				tempBuf[endIdx] = '\0';
				idx = endIdx + startIdx + 1;

				isEmptyTag = false;
				if(tempBuf[endIdx-1] == '/') isEmptyTag = true;

				if(tempBuf[0] == '?') cout << "Processing Instruction" << endl;
				else if(tempBuf[0] == '!') cout << "DTD" << endl;
				else if(tempBuf[0] == '/')
				{
					XpathRoute = XpathRoute->getParentNode();
				}
				else
				{//일반태그 처리
					int blankNum = checkBlank(&tempBuf[0]);
					strncpy(tempElement, &tempBuf[0], blankNum);
					tempElement[blankNum] = '\0';
					if(isRoute)
					{
						isRoute = false;
						XpathRoute->setParentNode(XpathRoute);
						XpathRoute->setTagName(tempElement);
					}
					else
					{
						XMLNode temp;
						XMLNode* currentNode = XpathRoute;
						temp.setTagName(tempElement);
						XpathRoute->setChildNode(&temp);
						XpathRoute = &XpathRoute->getChildNode()->back();
						XpathRoute->setParentNode(currentNode);

						if(isEmptyTag) XpathRoute = XpathRoute->getParentNode();
					}
					//================attribute 처리기능 추가=============
				}
			}
			else if(checkAlpha(buf[idx]) || checkNumber(buf[idx]))
			{//Content 처리
				endIdx = checkStartPoint(&buf[idx]);
				strncpy(tempBuf, &buf[idx], endIdx);
				tempBuf[endIdx] = '\0';
				idx = idx + endIdx;
				XpathRoute->setContent(tempBuf);
			}
			else
			{
				cout << "error2" << endl;
				cout << buf << endl;
				return 2;
			}
		}
	}

	fin.close();

	while(1)
	{
		cout << "cmd : ";
		cin >> cmdBuf;

		if(!strcmp(cmdBuf, "quit")) break;
		else if(!strcmp(cmdBuf, "up")) XpathRoute = XpathRoute->getParentNode();
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
			for(iter = XpathRoute->getChildNode()->begin(); iter != XpathRoute->getChildNode()->end(); iter++)
			{
				cout << &(*iter->getTagName()) << " ";
			}
			cout << endl;
		}
		else cout << "cmd error" << endl;

	}

	delete[] fileName;
	delete[] tempElement;
	delete[] cmdBuf;
	delete[] buf;
	delete[] tempBuf;

	/*
	cout << "press any key.." << endl;
	cin.ignore();
	cin.get();
	*/

	return 0;
}
