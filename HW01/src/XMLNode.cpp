/*
 * XMLNode.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: 신동환
 */

#include "XMLNode.h"

XMLNode::XMLNode() {
	name = new char[MAX_CHAR_SIZE];
	value = new char[MAX_CHAR_SIZE];
	parentNode = NULL;
	name[0] = '\0';
	value[0] = '\0';
}

//default copy constructor. list의 push 때문에 작성.
XMLNode::XMLNode(const XMLNode& node)
{
	name = new char[MAX_CHAR_SIZE];
	value = new char[MAX_CHAR_SIZE];
	name[0] = '\0';
	value[0] = '\0';

	strcpy(name, node.name);
	strcpy(value, node.value);
	parentNode = node.parentNode;
	childNode = node.childNode;
	attribute = node.attribute;
}

XMLNode::~XMLNode() {
	delete[] name;
	delete[] value;
}

//문자열str 에서 _ch문자가 있는지 검사하는 함수. 문자열 처음부터 _last 문자가 나타날때 까지 루프를 반복함.
int XMLNode::checkAnyChar(const char* str, const char _ch, const char _last)
{
	int _idx = 0;
	while (str[_idx] != _last)
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	//_last까지 검사하였는데 &가 나타나지 않았을경우.
	//&quot; 등을 검사하는 checkAmp() 함수에서 벗어나기 위해 존재함.
	if(_ch == '&') return -2;

	return _idx;
}

//문자열str에 &quot;와 같은 Entity들을 검사하고 바꿔주는 함수.
//이 함수를 거치면 &quot; -> " 이런 형태로 문자가 바뀜.
char* XMLNode::checkAmp(char* str)
{
	int _idx = checkAnyChar(str, '&','\0');
	if(_idx == -2) return str;

	int ampEnd;
	int charEnd;

	while(1)
	{
		//aaa&quot;bbb -> 기본 형태. &quot;를 예제로 설명.
		char* tempCheckAmp = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp2 = new char[MAX_CHAR_SIZE];
		char* tempCheckAmp3 = new char[MAX_CHAR_SIZE];

		ampEnd	= checkAnyChar(str, ';', '\0');	//;가 존재하는지 검사
		charEnd	= checkAnyChar(str, '\0', '\0');//문자열 끝나는 지점 검사

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
			std::cout << str << std::endl;
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

		//남은 문자열에서 &가 있는지 검사하고, 없을경우 break; 기호로 수정한 이후의 문자열을 검사하기 때문에 &amp; -> &변환에 대해 안전함
		if(checkAnyChar(&str[_idx+1], '&', '\0') == -2) break;
		_idx = _idx + checkAnyChar(&str[_idx+1], '&', '\0') + 1;//새로운 & 시작점을 찾아서 _idx에 저장
	}

	return str;
}

char* XMLNode::getName()
{
	return name;
}

void XMLNode::setName(const char* _name)
{
	strcpy(name, _name);
	checkAmp(name);
}

char* XMLNode::getValue()
{
	return value;
}

void XMLNode::setValue(const char* _value)
{
	//저장할 문자열이 한 줄이 아니라 여러줄에 걸쳐 작성되었을때 한칸 띄우고 붙여서 저장하기 위함.
	if(value[0] != '\0')
	{
		strcat(value, " ");
		strcat(value, _value);
	}

	else strcpy(value, _value);

	checkAmp(value); //저장한 문자열에 &quot; 같은 것이 있는지 검사
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

std::list<XMLNode>* XMLNode::getAttribute()
{
	return &attribute;
}

void XMLNode::setAttribute(const XMLNode* _attribute)
{
	attribute.push_back(*_attribute);
}

void XMLNode::PrintValue()
{
	std::cout << getValue() << std::endl;
}

void XMLNode::PrintName()
{
	std::cout << getName() << std::endl;
}

void XMLNode::PrintNode(PrintType _type)
{
	if(_type == print_Name) 		PrintName();
	else if(_type == print_Value)	PrintValue();
}
