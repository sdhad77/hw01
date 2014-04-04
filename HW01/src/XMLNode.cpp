/*
 * XMLNode.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: 신동환
 */

#include "XMLNode.h"

XMLNode::XMLNode() {
	tagName = new char[MAX_CHAR_SIZE];
	content = new char[MAX_CHAR_SIZE];
	parentNode = 0;
	tagName[0] = '\0';
	content[0] = '\0';
}

XMLNode::XMLNode(const XMLNode& node)
{
	tagName = new char[MAX_CHAR_SIZE];
	content = new char[MAX_CHAR_SIZE];
	tagName[0] = '\0';
	content[0] = '\0';

	strcpy(tagName, node.tagName);
	strcpy(content, node.content);
	parentNode = node.parentNode;
}

XMLNode::~XMLNode() {
	delete[] tagName;
	delete[] content;
}

int XMLNode::checkChar(const char* str, const char _ch)
{
	int _idx = 0;
	while (str[_idx] != '\0')
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	if(_ch == '&') return -2;

	return _idx;
}

char* XMLNode::checkAmp(char* str)
{
	int _idx = checkChar(str, '&');
	if(_idx == -2) return str;

	int ampEnd;
	int charEnd;

	while(1)
	{	//aaa&quot;bbb -> 예시형태
		char* tempCheckAmp = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp2 = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp3 = new char[MAX_CHAR_SIZE];

		ampEnd = checkChar(str, ';');	//;위치 반환
		charEnd = checkChar(str, '\0');	//문자열의 끝 반환

		strncpy(tempCheckAmp, &str[_idx+1], ampEnd -_idx - 1); //&quot; 중에서 quot 복사
		tempCheckAmp[ampEnd -_idx - 1] = '\0';	//quot 뒤에 \0 붙임

		if(!strcmp(tempCheckAmp, "lt"))			tempCheckAmp = (char*)"<";
		else if(!strcmp(tempCheckAmp, "gt"))	tempCheckAmp = (char*)">";
		else if(!strcmp(tempCheckAmp, "amp"))	tempCheckAmp = (char*)"&";
		else if(!strcmp(tempCheckAmp, "apos"))	tempCheckAmp = (char*)"'";
		else if(!strcmp(tempCheckAmp, "quot"))	tempCheckAmp = (char*)"\"";
		else
		{
			std::cout << "기본 제공 Entity가 아닙니다." << std::endl;
			break;
		}
																		//aaa&quot;bbb -> 예시형태
		strncpy(tempCheckAmp3, str, _idx); tempCheckAmp3[_idx] = '\0';	//aaa 복사
		strncpy(tempCheckAmp2, &str[ampEnd+1], charEnd-ampEnd);			//bbb 복사
		strcat(tempCheckAmp3, tempCheckAmp);							//aaa' 결합
		strcat(tempCheckAmp3, tempCheckAmp2);							//aaa'bbb 결합
		strcpy(str, tempCheckAmp3);										//aaa'bbb 복사

		delete[] tempCheckAmp;
		delete[] tempCheckAmp2;
		delete[] tempCheckAmp3;

		if(checkChar(&str[_idx+1], '&') == -2) break;	//& 못찾으면 break
		_idx = _idx + checkChar(&str[_idx+1], '&') + 1;	//&있을경우 &위치를 idx에 저장
	}

	return str;
}

char* XMLNode::getTagName()
{
	return tagName;
}

void XMLNode::setTagName(const char* _tagName)
{
	strcpy(tagName, _tagName);
	checkAmp(tagName);
}

char* XMLNode::getContent()
{
	return content;
}

void XMLNode::setContent(const char* _content)
{
	if(content[0] != '\0')
	{
		strcat(content, " ");
		strcat(content, _content);
	}
	else
	{
		strcpy(content, _content);
		checkAmp(content);
	}
}

XMLNode* XMLNode::getParentNode()
{
	return parentNode;
}
void XMLNode::setParentNode(XMLNode* _parentNode)
{
	parentNode = _parentNode;
}

std::list<XMLNode>* XMLNode::getChildNode()
{
	return &childNode;
}

void XMLNode::setChildNode(const XMLNode* _childNode)
{
	childNode.push_back(*_childNode);
}

std::list<tagAttribute>* XMLNode::getAttribute()
{
	return &attribute;
}

void XMLNode::setAttribute(const tagAttribute* _attribute)
{
	attribute.push_back(*_attribute);
}
