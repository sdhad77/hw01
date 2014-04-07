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

enum printType { print_TagName, print_Content, print_AtrName, print_AtrValue};

struct tagAttribute
{
private:
	char* name;
	char* value;

public:
	tagAttribute()
	{
		name = new char[MAX_CHAR_SIZE];
		value = new char[MAX_CHAR_SIZE];
		name[0] = '\0';
		value[0] = '\0';
	}
	tagAttribute(const tagAttribute& attribute)
	{
		name = new char[MAX_CHAR_SIZE];
		value = new char[MAX_CHAR_SIZE];
		name[0] = '\0';
		value[0] = '\0';

		strcpy(name, attribute.name);
		strcpy(value, attribute.value);
	}
	char* getName() {return name;}
	void setName(const char* _name) {strcpy(name, _name);}
	char* getValue() {return value;}
	void setValue(const char* _value) {strcpy(value, _value);}
	void PrintName() {std::cout << name << std::endl;}
	void PrintValue() {std::cout << value << std::endl;}
};

class XMLNode {
public:
	XMLNode();
	XMLNode(const XMLNode& node);
	virtual ~XMLNode();

private:
	char* tagName;
	char* content;
	XMLNode* parentNode;
	std::list<tagAttribute> attribute;
	std::list<XMLNode> childNode;

public:
	int checkChar(const char* str, const char _ch);
	char* checkAmp(char* str);

	char* getTagName();
	void setTagName(const char* _tagName);
	char* getContent();
	void setContent(const char* _content);
	XMLNode* getParentNode();
	void setParentNode(XMLNode* _parentNode);
	std::list<XMLNode>* getChildNode();
	void setChildNode(const XMLNode* _childNode);
	std::list<tagAttribute>* getAttribute();
	void setAttribute(const tagAttribute* _attribute);

	void PrintContent();
	void PrintTagName();
	void PrintAttribute();
	void PrintNode(printType _type);
};

#endif /* XMLNODE_H_ */
