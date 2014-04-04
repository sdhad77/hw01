/*
 * XMLParser.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: �ŵ�ȯ
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
	isRoute 	= 	true; //��Ʈ �������
	isEmptyTag 	= 	false;//���±�����

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

int XMLParser::checkChar(const char* str, const char _ch)
{
	int _idx = 0;
	while (str[_idx] != '\0')
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	if(_ch == '>') return -1; // �±� �ݴ� ��ȣ�� ã�� �������� ���� ó���� ����

	return _idx;
}

void XMLParser::parserPI()
{
	cout << "Processing Instruction" << endl;
}

void XMLParser::parserDTD()
{
	cout << "DTD" << endl;
}

void XMLParser::parserStartTag()
{
	int blankNum = checkChar(&tempBuf[0], ' ');
	strncpy(tempElement, &tempBuf[0], blankNum);
	tempElement[blankNum] = '\0';

	isEmptyTag = false;
	if(tempBuf[endIdx-1] == '/') isEmptyTag = true;

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
	}
	parserAttribute(blankNum); // attribute ó��

	if(isEmptyTag) XpathRoute = XpathRoute->getParentNode(); // ���±� ó��
}

void XMLParser::parserEndTag()
{
	XpathRoute = XpathRoute->getParentNode();
}

void XMLParser::parserContent()
{
	endIdx = checkChar(&buf[idx], '<');
	strncpy(tempBuf, &buf[idx], endIdx);
	tempBuf[endIdx] = '\0';
	idx = idx + endIdx;
	XpathRoute->setContent(tempBuf);
}

void XMLParser::parserAttribute(int _blankNum)
{
	while(1)
	{
		while(tempBuf[_blankNum] == ' ') _blankNum++; // ���鹮�� ����

		if(tempBuf[_blankNum] != '\0')
		{
			tagAttribute tempAttribute;
			int _startIdx = _blankNum;
			int _endIdx = checkChar(&tempBuf[_startIdx], '=');

			strncpy(tempAttributeName, &tempBuf[_startIdx], _endIdx);
			tempAttributeName[_endIdx] = '\0';
			tempAttribute.setName(tempAttributeName);

			_startIdx = _startIdx + _endIdx + 2;
			_endIdx = checkChar(&tempBuf[_startIdx], '"');

			strncpy(tempAttributeValue, &tempBuf[_startIdx], _endIdx);
			tempAttributeValue[_endIdx] = '\0';
			tempAttribute.setValue(tempAttributeValue);

			XpathRoute->setAttribute(&tempAttribute);

			_blankNum = _startIdx + _endIdx + 1;
		}
		if(tempBuf[_blankNum] == '\0') break;
		else if(tempBuf[_blankNum] == '/' && tempBuf[_blankNum+1] == '\0') break;
	}
}

int XMLParser::parser(const char* fileName, XMLNode* _XMLNode)
{
	XpathRoute = _XMLNode;

	ifstream fin(fileName);
	if(!fin)
	{
		cout << "�������� �ʴ� �����Դϴ�." << endl;
		return 1;
	}

	while(fin.getline(buf, MAX_BUF_SIZE))
	{
		idx = 0;
		while(buf[idx] != '\0')
		{
			while(buf[idx] == ' ') idx++; // ���鹮�� ����

			if(buf[idx] == '<')
			{
				idx++;
				startIdx = idx;
				endIdx = checkChar(&buf[startIdx], '>');
				while(endIdx == -1)
				{
					fin.getline(tempBuf, MAX_BUF_SIZE);
					strcat(buf,tempBuf);
					endIdx = checkChar(&buf[startIdx], '>');
				}
				strncpy(tempBuf, &buf[startIdx], endIdx);
				tempBuf[endIdx] = '\0';
				idx = endIdx + startIdx + 1; //���� �±��� ��+1�� �ε��� �̵���Ŵ

				if		(tempBuf[0] == '?')	parserPI();
				else if	(tempBuf[0] == '!')	parserDTD();
				else if	(tempBuf[0] == '/')	parserEndTag();
				else						parserStartTag();
			}
			else if(checkAlpha(buf[idx]) || checkNumber(buf[idx])) parserContent();
			else
			{
				cout << "���������� ���� ���ڿ��� �ԷµǾ����ϴ�." << endl;
				cout << buf << endl;
				return 2;
			}
		}
	}

	fin.close();

	return 0;
}
