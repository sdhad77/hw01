/*
 * XMLNode.h
 *
 *  Created on: 2014. 4. 2.
 *      Author: ½Åµ¿È¯
 */
#include <iostream>
#include<list>
#include<string>

#define MAX_CHAR_SIZE 100
#define MAX_BUF_SIZE 500

struct tagAttribute
{
	char* name;
	char* value;
	tagAttribute()
	{
		name = new char[MAX_CHAR_SIZE];
		value = new char[MAX_CHAR_SIZE];
	}
};


#ifndef XMLNODE_H_
#define XMLNODE_H_

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
	char* getTagName();
	void setTagName(const char* _tagName);
	char* getContent();
	void setContent(const char* _content);
	XMLNode* getParentNode();
	void setParentNode(XMLNode* _parentNode);
	std::list<XMLNode>* getChildNode();
	void setChildNode(const XMLNode* _childNode);
};

#endif /* XMLNODE_H_ */
