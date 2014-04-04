/*
 * XMLNode.cpp
 *
 *  Created on: 2014. 4. 2.
 *      Author: ½Åµ¿È¯
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

char* XMLNode::getTagName()
{
	return tagName;
}

void XMLNode::setTagName(const char* _tagName)
{
	strcpy(tagName, _tagName);
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
	else strcpy(content, _content);
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
