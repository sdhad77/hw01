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
	cmdIdx = 0;
	cmdBuf = new char[MAX_BUF_SIZE];
	strBuf = new char[MAX_CHAR_SIZE];
}

XPath::~XPath() {
	delete[] cmdBuf;
	delete[] strBuf;
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

//str 에서 _ch문자가 있는지 검사하는 함수. 문자열 처음부터 _last 문자가 나타날때 까지 루프를 반복함.
int XPath::checkAnyChar(const char* str, const char _ch, const char _last)
{
	int _idx = 0;
	while (str[_idx] != _last)
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	return -1;
}

//공백제거
void XPath::RemoveBlank()
{
	while(cmdBuf[cmdIdx] == ' ' || cmdBuf[cmdIdx] == '\t') cmdIdx++;
}

//노드 저장용 큐 비우기
void XPath::ClearQ()
{
	while(searchNodeQ.size()) searchNodeQ.pop();
}

//_num 만큼 pop() 시키기
void XPath::PopQ(int _num)
{
	int temp = _num;
	while(searchNodeQ.size() && (temp--)) searchNodeQ.pop();
}

//cmd버퍼로부터 단어단위로 잘라서 strBuf에 저장.
void XPath::StrCpyFromCmdBuf()
{
	int _startIdx = cmdIdx;
	while(checkAlpha(cmdBuf[cmdIdx])) cmdIdx++;
	strncpy(strBuf, &cmdBuf[_startIdx], cmdIdx-_startIdx);
	strBuf[cmdIdx-_startIdx] = '\0';
}

//cmd버퍼로부터 숫자를 잘라서 strBuf에 저장.
//현재 int만 가능, 실수형도 가능하게끔 변경해야함.
void XPath::NumberCpyFromCmdBuf()
{
	int _startIdx = cmdIdx;
	while(checkNumber(cmdBuf[cmdIdx])) cmdIdx++;
	strncpy(strBuf, &cmdBuf[_startIdx], cmdIdx-_startIdx);
	strBuf[cmdIdx-_startIdx] = '\0';
}

//command 읽는중에 발생하는 에러들을 한곳에 모아놓음.
void XPath::ErrorCollection(const char* str)
{
	if(!strcmp(str, "cmd"))			std::cout << "cmd를 다시 입력해 주세요." << std::endl;
	else if(!strcmp(str, "//@"))	std::cout << "잘못입력하였습니다." << std::endl;
	else if(!strcmp(str, "//"))		std::cout << "// 이후 알파벳,*,@ 이외의 문자가 입력되었습니다." << std::endl;
	else if(!strcmp(str, "/"))		std::cout << "/ 이후 처리되지 않은 문자가 나타났습니다." << std::endl;
	else if(!strcmp(str, "[999]"))	std::cout << "입력된 숫자가 너무 큽니다." << std::endl;
	else if(!strcmp(str, "[999"))	std::cout << "[로 시작하면 ]로 끝나야 합니다." << std::endl;
	else if(!strcmp(str, "[func("))	std::cout << ") 는 어디에.." << std::endl;
	else if(!strcmp(str, "[~"))		std::cout << "[ 이후 처리되지 않은 문자가 입력되었습니다." << std::endl;
	else if(!strcmp(str, "[name~$"))std::cout << "[a 이후 처리되지 않은 문자가 입력되었습니다." << std::endl;
	else							std::cout << "error" << std::endl;

	cmdBuf[cmdIdx] = '\0';
	ClearQ();
}

//입력받은 커맨드를 분석하고 실행함.
//자료의 형태는 트리이며,
//탐색 방식은 크게 두가지가 존재함.
//1.재귀호출을 이용한 트리 전체 순회 방식
//2.큐를 이용한 깊이별 탐색 방식
int XPath::XPathCmdParser(char* _cmdBuf, XMLNode* _XpathRoute)
{
	cmdIdx = 0;
	strcpy(cmdBuf, _cmdBuf);

	ClearQ(); //탐색한 노드 저장용 큐 초기화

	//루트 부터 탐색하기 위해 루트의 부모 노드가 필요함.
	XMLNode* tempNode = new XMLNode;
	tempNode->setName("Root's Parent"); //이름 설정
	tempNode->setChildNode(_XpathRoute);//루트를 자식으로 설정
	searchNodeQ.push(tempNode);//큐에 push함.

	while(cmdBuf[cmdIdx] != '\0')
	{
		RemoveBlank();

		if(cmdBuf[cmdIdx] == '/')
		{
			if(cmdBuf[cmdIdx+1] == '/')
			{
				if(cmdBuf[cmdIdx+2] == '*')
				{
					cmdIdx = cmdIdx + 3;
					Search_All_NonString(_XpathRoute);//문자열에 관계없이 루트노드부터 전부 저장.
					printType = print_Name;
				}
				else if(cmdBuf[cmdIdx+2] == '@')
				{
					if(checkAlpha(cmdBuf[cmdIdx+3]))
					{
						cmdIdx = cmdIdx + 3;//cmd버퍼의 인덱스를 첫번째 알파벳으로 위치시킴.
						StrCpyFromCmdBuf();	//cmd버퍼에서 단어단위로 잘라서 복사함.
						RemoveBlank();		//잘라낸 이후에 공백이 있을수도 있으니 cmd버퍼의 공백 제거
						ClearQ();			//노드를 저장할 큐를 비움.
											//루트부터 잘라낸 str과 일치하는 속성이름을 검색하여 큐에 저장함.
						Search_All(_XpathRoute, strBuf, search_AttributeName);
						printType = print_Value;//출력값을 value로 설정.
					}
					else ErrorCollection("//@");
				}
				else if(checkAlpha(cmdBuf[cmdIdx+2]))
				{
					cmdIdx = cmdIdx + 2;
					StrCpyFromCmdBuf();
					RemoveBlank();
					ClearQ();
					Search_All(_XpathRoute, strBuf, search_TagName);
					printType = print_Value;
				}
				else ErrorCollection("//");
			}
			else if(cmdBuf[cmdIdx+1] == '*')
			{
				int tempQSize = searchNodeQ.size();
				while(tempQSize--)
				{
					Search_All_NonString(searchNodeQ.front());
					searchNodeQ.pop();
				}
				cmdIdx = cmdIdx + 2;
				printType = print_Name;
			}
			else if(checkAlpha(cmdBuf[cmdIdx+1]))
			{
				cmdIdx = cmdIdx + 1;
				StrCpyFromCmdBuf();
				Search_Child(strBuf);
				printType = print_Value;
			}
			else ErrorCollection("/");
		}
		else if(cmdBuf[cmdIdx] == '[')
		{
			if(checkNumber(cmdBuf[cmdIdx+1]))
			{
				cmdIdx = cmdIdx + 1;
				NumberCpyFromCmdBuf();
				RemoveBlank();

				if(cmdBuf[cmdIdx] == ']')
				{
					cmdIdx++;
					int selectCnt = atoi(strBuf);
					if((int)searchNodeQ.size() >= selectCnt)
					{
						while(--selectCnt) searchNodeQ.pop();
						searchNodeQ.push(searchNodeQ.front());
						while(searchNodeQ.size()-1) searchNodeQ.pop();
						printType = print_Value;
					}
					else ErrorCollection("[999]");
				}
				else ErrorCollection("[999");
			}
			else if(checkAlpha(cmdBuf[cmdIdx+1]))
			{
				if(checkAnyChar(&cmdBuf[cmdIdx+1], '(', ']'))
				{
					int checkFunc = checkAnyChar(&cmdBuf[cmdIdx+1], '(', ']');

					if(cmdBuf[cmdIdx + 1 + checkFunc + 1] == ')')
					{
						std::cout << "함수" << std::endl;
						cmdBuf[cmdIdx] = '\0';
					}
					else ErrorCollection("[func(");
				}
				else ErrorCollection("[name~$");
			}
			else if(cmdBuf[cmdIdx] == '@')
			{
				cmdBuf[cmdIdx] = '\0';
			}
			else ErrorCollection("[~");
		}
		else ErrorCollection("cmd");
	}

	PrintNodeQ();

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
