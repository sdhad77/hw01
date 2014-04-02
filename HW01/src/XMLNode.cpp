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
}

XMLNode::XMLNode(const XMLNode& node)
{
	tagName = new char[MAX_CHAR_SIZE];
	content = new char[MAX_CHAR_SIZE];

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

char* XMLNode::getContentName()
{
	return content;
}

void XMLNode::setContentName(const char* _content)
{
	strcpy(content, _content);
}

XMLNode* XMLNode::getParentNode()
{
	return parentNode;
}
void XMLNode::setParentNode(XMLNode* _parentNode)
{
	parentNode = _parentNode;
}

XMLNode* XMLNode::getChildNode()
{
	return &childNode.back();
}

void XMLNode::setChildNode(const XMLNode* _childNode)
{
	childNode.push_back(*_childNode);
}
