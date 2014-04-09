/*
 * XMLParser.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: 신동환
 */

#include "XMLParser.h"

XMLParser::XMLParser() {
	tempElement			=	new char[MAX_CHAR_SIZE];
	tempAttributeName	=	new char[MAX_CHAR_SIZE];
	tempAttributeValue	=	new char[MAX_CHAR_SIZE];
	buf					= 	new char[MAX_BUF_SIZE];
	tempBuf				= 	new char[MAX_BUF_SIZE];

	idx 		= 	0;
	startIdx 	= 	0;
	endIdx 		= 	0;
	isRoute 	= 	true; //루트 노드인지 검사하기 위함
	isEmptyTag 	= 	false;//빈태그인지 검사하기 위함

	XpathRoute = new XMLNode;
}

XMLParser::~XMLParser() {
	delete[] tempElement;
	delete[] tempAttributeName;
	delete[] tempAttributeValue;
	delete[] buf;
	delete[] tempBuf;
}

bool XMLParser::checkAlpha(const char ch)
{
	if ('a' <= ch && ch <= 'z') return true;
	else if ('A' <= ch && ch <= 'Z') return true;
	else return false;
}

bool XMLParser::checkNumber(const char ch)
{
	if ('0' <= ch && ch <= '9') return true;
	else return false;
}

int XMLParser::checkAnyChar(const char* str, const char _ch, const char _last)
{
	int _idx = 0;
	while (str[_idx] != _last)
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	if(_ch == '>') return -1; // >태그가 등장하지 않았을 경우, 다음 라인을 추가로 읽어오기 위해 존재함.
	if(_ch == '&') return -2; //&quot; 등을 검사하는 checkAmp() 함수에서 벗어나기 위해 존재함.
	if(_ch == '"') return -3; // "기호가 아닌 '기호로 시작되는 attribute value를 검사하기 위함.

	return _idx;
}

//문자열str에 &quot;와 같은 Entity들을 검사하고 바꿔주는 함수.
//이 함수를 거치면 &quot; -> " 이런 형태로 문자가 바뀜.
char* XMLParser::checkAmp(char* str)
{
	int _idx = checkAnyChar(str, '&','\0');
	if(_idx == -2) return str;

	int ampEnd;
	int charEnd;

	while(1)
	{
		//aaa&quot;bbb -> 기본 형태. &quot;을 예제로 설명.
		char* tempCheckAmp = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp2 = new char[MAX_CHAR_SIZE];

		ampEnd	= checkAnyChar(str, ';', '\0');	//;가 존재하는지 검사
		charEnd	= checkAnyChar(str, '\0', '\0');//문자열 끝나는 지점 검사

		strncpy(tempCheckAmp, &str[_idx+1], ampEnd -_idx - 1); //&quot; -> quot 만 따로 저장
		tempCheckAmp[ampEnd -_idx - 1] = '\0';	//quot 뒤에 \0 붙여줌

		strncpy(tempCheckAmp2, &str[ampEnd+1], charEnd-ampEnd);  //bbb만 분리

		//문자열에서 &와 ;사이에 있던 문자열이 무엇인지 검사 후 기호로 변경
		if(!strcmp(tempCheckAmp, "lt"))			str[_idx] = '<';
		else if(!strcmp(tempCheckAmp, "gt"))	str[_idx] = '>';
		else if(!strcmp(tempCheckAmp, "amp"))	str[_idx] = '&';
		else if(!strcmp(tempCheckAmp, "apos"))	str[_idx] = '\'';
		else if(!strcmp(tempCheckAmp, "quot"))	str[_idx] = '"';
		else
		{
			std::cout << "존재하지 않는 Entity입니다." << std::endl;
			std::cout << str << std::endl;
			break;
		}

		//기호로 변경한 문자 바로 뒤에 아까 분리한 bbb 부분을 이어 붙임
		strncpy(&str[_idx+1], tempCheckAmp2, charEnd-ampEnd);

		delete[] tempCheckAmp;
		delete[] tempCheckAmp2;

		//남은 문자열에서 &가 있는지 검사하고, 없을경우 break; 기호로 수정한 이후의 문자열을 검사하기 때문에 &amp; -> &변환에 대해 안전함
		if(checkAnyChar(&str[_idx+1], '&', '\0') == -2) break;
		_idx = _idx + checkAnyChar(&str[_idx+1], '&', '\0') + 1;//새로운 & 시작점을 찾아서 _idx에 저장
	}

	return str;
}

bool XMLParser::checkByteOrderMark()
{
	//byte order mark**********************
	if((    (unsigned char)buf[0] == 0xEF)	//0xEFBBBF -> UTF-8의 byte order mark
		&& ((unsigned char)buf[1] == 0xBB)	//BOM이 존재할 경우 그대로 출력시키고
		&& ((unsigned char)buf[2] == 0xBF))	//다음 단어로 넘어갑니다.
	{
		idx = 3;							//BOM이후로 idx를 이동시킵니다.
		return true;
	}

	idx = 0;
	return false;
}

void XMLParser::parserPI()
{
	cout << "Processing Instruction" << endl;
}

void XMLParser::parserDTD()
{
	if(tempBuf[1] == '-' && tempBuf[2] == '-') parserComment();
	else cout << "DTD" << endl;
}

void XMLParser::parserComment()
{
	cout << "Comment" << endl;
}

void XMLParser::parserStartTag()
{
	int blankNum = checkAnyChar(&tempBuf[0], ' ', '\0');
	strncpy(tempElement, &tempBuf[0], blankNum);
	tempElement[blankNum] = '\0';
	checkAmp(tempElement);

	isEmptyTag = false;
	if(tempBuf[endIdx-1] == '/') isEmptyTag = true;

	if(isRoute)
	{
		isRoute = false;
		XpathRoute->setParentNode(XpathRoute);
		XpathRoute->setName(tempElement);
	}
	else
	{
		XMLNode temp;
		XMLNode* currentNode = XpathRoute;
		temp.setName(tempElement);
		XpathRoute->setChildNode(&temp);
		XpathRoute = &XpathRoute->getChildNode()->back();
		XpathRoute->setParentNode(currentNode);
	}
	parserAttribute(blankNum); // attribute 처리

	if(isEmptyTag) XpathRoute = XpathRoute->getParentNode(); // 빈 태그일경우, 부모 노드로 바로 복귀시킴
}

void XMLParser::parserEndTag()
{
	XpathRoute = XpathRoute->getParentNode();
}

void XMLParser::parserContent()
{
	endIdx = checkAnyChar(&buf[idx], '<', '\0');
	strncpy(tempBuf, &buf[idx], endIdx);
	tempBuf[endIdx] = '\0';
	idx = idx + endIdx;
	XpathRoute->setValue(tempBuf);
}

void XMLParser::parserAttribute(int _blankNum)
{
	while(1)
	{
		while(tempBuf[_blankNum] == ' ') _blankNum++; // 공백문자 제거

		if(tempBuf[_blankNum] != '\0')
		{
			XMLNode tempAttribute;
			int _startIdx = _blankNum;
			int _endIdx = checkAnyChar(&tempBuf[_startIdx], '=', '\0');

			strncpy(tempAttributeName, &tempBuf[_startIdx], _endIdx);
			tempAttributeName[_endIdx] = '\0';
			checkAmp(tempAttributeName);
			tempAttribute.setName(tempAttributeName);

			_startIdx = _startIdx + _endIdx + 2;
			_endIdx = checkAnyChar(&tempBuf[_startIdx], '"', '\0');
			if(_endIdx == -3) _endIdx = checkAnyChar(&tempBuf[_startIdx], '\'', '\0');

			strncpy(tempAttributeValue, &tempBuf[_startIdx], _endIdx);
			tempAttributeValue[_endIdx] = '\0';
			checkAmp(tempAttributeValue);
			tempAttribute.setValue(tempAttributeValue);

			XpathRoute->setAttribute(&tempAttribute);
			XpathRoute->getAttribute()->back().setParentNode(XpathRoute);

			_blankNum = _startIdx + _endIdx + 1;
		}

		while(tempBuf[_blankNum] == ' ') _blankNum++; // 공백문자 제거

		if(tempBuf[_blankNum] == '\0') break; //
		else if(tempBuf[_blankNum] == '/' && tempBuf[_blankNum+1] == '\0') break; //빈 태그일 경우 멈추기 위함.
	}
}

int XMLParser::parser(const char* fileName, XMLNode* _XMLNode)
{
	XpathRoute = _XMLNode;

	ifstream fin(fileName);
	if(!fin)
	{
		cout << "파일이 존재하지 않습니다." << endl;
		return 1;
	}

	//byte order mark 검사==================
	fin.getline(buf, MAX_BUF_SIZE);
	if(checkByteOrderMark())fin.seekg(0);
	//====================================

	while(fin.getline(buf, MAX_BUF_SIZE))
	{
		while(buf[idx] != '\0')
		{
			while(buf[idx] == ' ' || buf[idx] == '\t') idx++; // 공백이나 탭문자가 나타나면 idx 증가.

			if(buf[idx] == '<')
			{
				idx++;
				startIdx = idx;
				endIdx = checkAnyChar(&buf[startIdx], '>', '\0');
				while(endIdx == -1)
				{
					fin.getline(tempBuf, MAX_BUF_SIZE);
					strcat(buf,tempBuf);
					endIdx = checkAnyChar(&buf[startIdx], '>', '\0');
				}
				strncpy(tempBuf, &buf[startIdx], endIdx);
				tempBuf[endIdx] = '\0';
				idx = endIdx + startIdx + 1; //다음 문자열로 idx를 이동시킴

				if		(tempBuf[0] == '?')	parserPI();
				else if	(tempBuf[0] == '!') parserDTD(); // 주석은 parserDTD 함수 내에서 처리
				else if	(tempBuf[0] == '/')	parserEndTag();
				else						parserStartTag();
			}
			else if(checkAlpha(buf[idx]) || checkNumber(buf[idx])) parserContent();
			else
			{
				parserContent();
				cout << "알파벳과 숫자 이외의 문자가 입력되었습니다." << endl;
				cout << buf << endl;
			}
		}

		idx = 0;
	}

	fin.close();

	return 0;
}
