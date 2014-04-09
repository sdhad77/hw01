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

char* XMLNode::getName()
{
	return name;
}

void XMLNode::setName(const char* _name)
{
	strcpy(name, _name);
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
