/*
 * XPath.cpp
 *
 *  Created on: 2014. 4. 7.
 *      Author: 신동환
 */

#include "XPath.h"

XPath::XPath() {
	printType = print_Value;
	firstCallSearch_All = true;
}

XPath::~XPath() {
}

bool XPath::checkAlpha(const char ch)
{
	if ('a' <= ch && ch <= 'z') return true;
	else if ('A' <= ch && ch <= 'Z') return true;
	else return false;
}

bool XPath::checkNumber(const char ch)
{
	if ('0' <= ch && ch <= '9') return true;
	else return false;
}

int XPath::XPathCmdParser(char* _cmdBuf, XMLNode* _XpathRoute)
{
	int _cmdIdx = 0;
	int _startIdx = 0;
	char* _strBuf = new char[MAX_CHAR_SIZE];

	while(searchNodeQ.size()) searchNodeQ.pop();

	XMLNode* tempNode = new XMLNode;
	tempNode->setName("Root's Parent");
	tempNode->setChildNode(_XpathRoute);
	searchNodeQ.push(tempNode);

	while(_cmdBuf[_cmdIdx] != '\0')
	{
		while(_cmdBuf[_cmdIdx] == ' ') _cmdIdx++; //공백제거

		if(_cmdBuf[_cmdIdx] == '/')
		{
			if(_cmdBuf[_cmdIdx+1] == '/')
			{
				if(_cmdBuf[_cmdIdx+2] == '*')
				{
					Search_All_NonString(_XpathRoute);
					_cmdIdx = _cmdIdx + 3;
					printType = print_Name;
				}
				else if(_cmdBuf[_cmdIdx+2] == '@')
				{
					if(checkAlpha(_cmdBuf[_cmdIdx+3]))
					{
						_cmdIdx = _cmdIdx + 3;
						_startIdx = _cmdIdx;
						while(checkAlpha(_cmdBuf[_cmdIdx])) _cmdIdx++;
						strncpy(_strBuf, &_cmdBuf[_startIdx], _cmdIdx-_startIdx);
						_strBuf[_cmdIdx-_startIdx] = '\0';

						while(_cmdBuf[_cmdIdx] == ' ') _cmdIdx++; //공백제거

						while(searchNodeQ.size()) searchNodeQ.pop();

						Search_All(_XpathRoute, _strBuf, search_AttributeName);
			//			int tempQSize = searchNodeQ.size(); //  "//"을 이용한 검색을 아무곳에서나 하고 싶으면 주석 해제하고 위에 Search_All 주석처리.
			//			for(int i=0; i < tempQSize; i++) Search_All(searchNodeQ.front(), _strBuf, search_AttributeName);
						printType = print_Value;
					}
					else
					{
						std::cout << "잘못입력하였습니다." << std::endl;
						_cmdBuf[_cmdIdx] = '\0';
					}
				}
				else if(checkAlpha(_cmdBuf[_cmdIdx+2]))
				{
					_cmdIdx = _cmdIdx + 2;
					_startIdx = _cmdIdx;
					while(checkAlpha(_cmdBuf[_cmdIdx])) _cmdIdx++;
					strncpy(_strBuf, &_cmdBuf[_startIdx], _cmdIdx-_startIdx);
					_strBuf[_cmdIdx-_startIdx] = '\0';

					while(_cmdBuf[_cmdIdx] == ' ') _cmdIdx++; //공백제거

					while(searchNodeQ.size()) searchNodeQ.pop();

					Search_All(_XpathRoute, _strBuf, search_TagName);
		//			int tempQSize = searchNodeQ.size(); //  "//"을 이용한 검색을 아무곳에서나 하고 싶으면 주석 해제하고 위에 Search_All 주석처리.
		//			for(int i=0; i < tempQSize; i++) Search_All(searchNodeQ.front(), _strBuf, search_TagName);
					printType = print_Value;
				}
				else
				{
					std::cout << "// 이후 알파벳,*,@ 이외의 문자가 입력되었습니다." << std::endl;
					_cmdBuf[_cmdIdx] = '\0';
				}
			}
			else if(_cmdBuf[_cmdIdx+1] == '*')
			{
				int tempQSize = searchNodeQ.size();
				while(tempQSize--)
				{
					Search_All_NonString(searchNodeQ.front());
					searchNodeQ.pop();
				}
				_cmdIdx = _cmdIdx + 2;
				printType = print_Name;
			}
			else if(checkAlpha(_cmdBuf[_cmdIdx+1]))
			{
				_startIdx = ++_cmdIdx;
				while(checkAlpha(_cmdBuf[_cmdIdx])) _cmdIdx++;
				strncpy(_strBuf, &_cmdBuf[_startIdx], _cmdIdx-_startIdx);
				_strBuf[_cmdIdx-_startIdx] = '\0';

				Search_Child(_strBuf);
				printType = print_Value;
			}
			else
			{
				std::cout << "/ 이후 처리되지 않은 문자가 나타났습니다." << std::endl;
			}
		}
		else if(_cmdBuf[_cmdIdx] == '[')
		{
			if(checkNumber(_cmdBuf[_cmdIdx+1]))
			{
				_startIdx = ++_cmdIdx;
				while(checkNumber(_cmdBuf[_cmdIdx])) _cmdIdx++;
				strncpy(_strBuf, &_cmdBuf[_startIdx], _cmdIdx-_startIdx);
				_strBuf[_cmdIdx-_startIdx] = '\0';

				while(_cmdBuf[_cmdIdx] == ' ') _cmdIdx++; //공백제거

				if(_cmdBuf[_cmdIdx] == ']')
				{
					_cmdIdx++;
					int selectCnt = atoi(_strBuf);
					if((int)searchNodeQ.size() >= selectCnt)
					{
						while(--selectCnt) searchNodeQ.pop();
						searchNodeQ.push(searchNodeQ.front());
						while(searchNodeQ.size()-1) searchNodeQ.pop();
						printType = print_Value;
					}
					else
					{
						std::cout << "입력된 숫자가 너무 큽니다." << std::endl;
						while(searchNodeQ.size()) searchNodeQ.pop();
					}
				}
				else
				{
					std::cout << "[로 시작하면 ]로 끝나야 합니다." << std::endl;
					while(searchNodeQ.size()) searchNodeQ.pop();
				}
			}
			else if(checkAlpha(_cmdBuf[_cmdIdx+1]))
			{
				while(_cmdBuf[_cmdIdx] != '\0') _cmdIdx++;
			}
			else
			{
				while(_cmdBuf[_cmdIdx] != '\0') _cmdIdx++;
			}
		}
		else
		{
			while(_cmdBuf[_cmdIdx] != '\0') _cmdIdx++;
		}
	}

	PrintNodeQ();

	delete[] _strBuf;
	delete tempNode;

	return 0;
}

void XPath::Search_All_NonString(XMLNode* _XpathRoute)
{
	firstCallSearch_All = true;

	Search_All(_XpathRoute);
}

void XPath::Search_All(XMLNode* _XpathRoute)
{
	if(!firstCallSearch_All) searchNodeQ.push(_XpathRoute);
	else firstCallSearch_All = false;

	std::list<XMLNode>::iterator _iter;
	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
			Search_All(&(*_iter));
}

void XPath::Search_All(XMLNode* _XpathRoute, const char* str, CommandType _commandType)
{
	std::list<XMLNode>::iterator _iter;
	std::list<XMLNode>::iterator _iter2;

	if(_commandType == search_TagName)
	{
		if(!strcmp(_XpathRoute->getName(), str)) searchNodeQ.push(_XpathRoute);
	}

	else if(_commandType == search_AttributeName)
	{
		for(_iter2 = _XpathRoute->getAttribute()->begin(); _iter2 != _XpathRoute->getAttribute()->end(); _iter2++)
		{
			if(!strcmp(_iter2->getName(), str)) searchNodeQ.push(&(*_iter2));
		}
	}

	for(_iter = _XpathRoute->getChildNode()->begin(); _iter != _XpathRoute->getChildNode()->end(); _iter++)
		Search_All(&(*_iter),str, _commandType);
}

//searchNodeQ 를 이용한 탐색
void XPath::Search_Child(const char* str)
{
	std::list<XMLNode>::iterator _iter;
	int tempCnt = searchNodeQ.size();

	while(tempCnt--)
	{
		for(_iter = searchNodeQ.front()->getChildNode()->begin(); _iter != searchNodeQ.front()->getChildNode()->end(); _iter++)
		{
			if(!strcmp(_iter->getName(),str)) searchNodeQ.push(&(*_iter));
		}
		searchNodeQ.pop();
	}
}

void XPath::AddNodeList(XMLNode* node)
{
	nodeList.push_back(node);
}

void XPath::ClearNodeList()
{
	nodeList.clear();
}

void XPath::PrintNodeList(PrintType _type)
{
	std::list<XMLNode *>::iterator _iter;
	for(_iter = nodeList.begin(); _iter != nodeList.end(); _iter++)
	{
		(*_iter)->PrintNode(_type);
	}
}

void XPath::PrintNodeQ()
{
	while(searchNodeQ.size())
	{
		searchNodeQ.front()->PrintNode(printType);
		searchNodeQ.pop();
	}
}
