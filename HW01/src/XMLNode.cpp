/*
 * XMLNode.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: �ŵ�ȯ
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
	{	//aaa&quot;bbb -> ��������
		char* tempCheckAmp = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp2 = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp3 = new char[MAX_CHAR_SIZE];

		ampEnd = checkChar(str, ';');	//;��ġ ��ȯ
		charEnd = checkChar(str, '\0');	//���ڿ��� �� ��ȯ

		strncpy(tempCheckAmp, &str[_idx+1], ampEnd -_idx - 1); //&quot; �߿��� quot ����
		tempCheckAmp[ampEnd -_idx - 1] = '\0';	//quot �ڿ� \0 ����

		if(!strcmp(tempCheckAmp, "lt"))			tempCheckAmp = (char*)"<";
		else if(!strcmp(tempCheckAmp, "gt"))	tempCheckAmp = (char*)">";
		else if(!strcmp(tempCheckAmp, "amp"))	tempCheckAmp = (char*)"&";
		else if(!strcmp(tempCheckAmp, "apos"))	tempCheckAmp = (char*)"'";
		else if(!strcmp(tempCheckAmp, "quot"))	tempCheckAmp = (char*)"\"";
		else
		{
			std::cout << "�⺻ ���� Entity�� �ƴմϴ�." << std::endl;
			break;
		}
																		//aaa&quot;bbb -> ��������
		strncpy(tempCheckAmp3, str, _idx); tempCheckAmp3[_idx] = '\0';	//aaa ����
		strncpy(tempCheckAmp2, &str[ampEnd+1], charEnd-ampEnd);			//bbb ����
		strcat(tempCheckAmp3, tempCheckAmp);							//aaa' ����
		strcat(tempCheckAmp3, tempCheckAmp2);							//aaa'bbb ����
		strcpy(str, tempCheckAmp3);										//aaa'bbb ����

		delete[] tempCheckAmp;
		delete[] tempCheckAmp2;
		delete[] tempCheckAmp3;

		if(checkChar(&str[_idx+1], '&') == -2) break;	//& ��ã���� break
		_idx = _idx + checkChar(&str[_idx+1], '&') + 1;	//&������� &��ġ�� idx�� ����
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
