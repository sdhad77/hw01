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
	{	//aaa&quot;bbb -> 기본 형태
		char* tempCheckAmp = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp2 = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp3 = new char[MAX_CHAR_SIZE];

		ampEnd = checkChar(str, ';');	//;가 존재하는지 검사
		charEnd = checkChar(str, '\0');	//문자열 끝나는 지점 검사

		strncpy(tempCheckAmp, &str[_idx+1], ampEnd -_idx - 1); //&quot; -> quot 만 따로 저장
		tempCheckAmp[ampEnd -_idx - 1] = '\0';	//quot 뒤에 \0 붙여줌

		//문자열에서 &와 ;사이에 있던 문자열이 무엇인지 검사 후 기호로 변경
		if(!strcmp(tempCheckAmp, "lt"))			tempCheckAmp = (char*)"<";
		else if(!strcmp(tempCheckAmp, "gt"))	tempCheckAmp = (char*)">";
		else if(!strcmp(tempCheckAmp, "amp"))	tempCheckAmp = (char*)"&";
		else if(!strcmp(tempCheckAmp, "apos"))	tempCheckAmp = (char*)"'";
		else if(!strcmp(tempCheckAmp, "quot"))	tempCheckAmp = (char*)"\"";
		else
		{
			std::cout << "존재하지 않는 Entity입니다." << std::endl;
			break;
		}
																		//aaa&quot;bbb -> 기본형태
		strncpy(tempCheckAmp3, str, _idx); tempCheckAmp3[_idx] = '\0';	//aaa만 분리
		strncpy(tempCheckAmp2, &str[ampEnd+1], charEnd-ampEnd);			//bbb만 분리
		strcat(tempCheckAmp3, tempCheckAmp);							//aaa' -> quot에 해당하는 기호를 aaa 뒤에 붙임
		strcat(tempCheckAmp3, tempCheckAmp2);							//aaa'bbb 위의 문자열들을 합침
		strcpy(str, tempCheckAmp3);										//aaa'bbb 합쳐진 문자열을 str로 카피

		delete[] tempCheckAmp;
		delete[] tempCheckAmp2;
		delete[] tempCheckAmp3;

		if(checkChar(&str[_idx+1], '&') == -2) break;	//남은 문자열에서 &가 있는지 검사하고, 없을경우 break; 기호로 수정한 이후의 문자열을 검사하기 때문에 &amp->& 변환에 대해 안전함
		_idx = _idx + checkChar(&str[_idx+1], '&') + 1;	//새로운 & 시작점을 찾아서 _idx에 저장
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
