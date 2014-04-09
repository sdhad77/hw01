/*
 * XMLNode.h
 *
 *  Created on: 2014. 4. 2.
 *      Author: 신동환
 */
#include <iostream>
#include <list>
#include <string>

#ifndef XMLNODE_H_
#define XMLNODE_H_

#define MAX_CHAR_SIZE 500
#define MAX_BUF_SIZE 800

enum PrintType { print_Name, print_Value};

class XMLNode {
public:
	XMLNode();
	XMLNode(const XMLNode& node); //default copy constructor. list의 push 때문에 작성.
	virtual ~XMLNode();

private:
	char* name;
	char* value;
	XMLNode* parentNode;
	std::list<XMLNode> attribute;
	std::list<XMLNode> childNode;

public:
	int checkAnyChar(const char* str, const char _ch, const char _last);
	char* checkAmp(char* str);

	char* getName();
	void setName(const char* _tagName);
	char* getValue();
	void setValue(const char* _content);
	XMLNode* getParentNode();
	void setParentNode(XMLNode* _parentNode);
	std::list<XMLNode>* getChildNode();
	void setChildNode(const XMLNode* _childNode);
	std::list<XMLNode>* getAttribute();
	void setAttribute(const XMLNode* _attribute);

	void PrintValue();
	void PrintName();
	void PrintNode(PrintType _type);
};

#endif /* XMLNODE_H_ */
