/*
 * XMLParser.h

 *
 *  Created on: 2014. 4. 2.
 *      Author: 신동환
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
	char* buf;			//input문서를 한 줄 단위로 저장하는 버퍼
	char* tempBuf;		//buf에서 태그 단위로 저장하는 버퍼

	int idx;
	int startIdx;
	int endIdx;
	bool isRoute;
	bool isEmptyTag;

	XMLNode* XpathRoute;
};

#endif /* XMLPARSER_H_ */
