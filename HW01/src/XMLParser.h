/*
 * XMLParser.h

 *
 *  Created on: 2014. 4. 2.
 *      Author: �ŵ�ȯ
 */
#include <iostream>
#include <fstream>
#include "XMLNode.h"
using namespace std;

#define MAX_CHAR_SIZE 500
#define MAX_BUF_SIZE 800

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

class XMLParser {
public:
	XMLParser();
	virtual ~XMLParser();

	bool checkAlpha(const char ch);
	bool checkNumber(const char ch);
	int checkChar(const char* str, const char _ch);
	int parser(const char* fileName, XMLNode* _XMLNode);
	void parserPI();
	void parserDTD();
	void parserComment();
	void parserStartTag();
	void parserEndTag();
	void parserContent();
	void parserAttribute(int _blankNum);

private:
	char* tempElement;
	char* tempAttributeName;
	char* tempAttributeValue;
	char* buf;			//input������ �� �� ������ �����ϴ� ����
	char* tempBuf;		//buf���� �±� ������ �����ϴ� ����

	int idx;
	int startIdx;
	int endIdx;
	bool isRoute;
	bool isEmptyTag;

	XMLNode* XpathRoute;
};

#endif /* XMLPARSER_H_ */
