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

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#define MAX_CHAR_SIZE 500
#define MAX_BUF_SIZE 800

class XMLParser {
public:
	XMLParser();
	virtual ~XMLParser();

private:
	char* tempElement;
	char* tempAttributeName;
	char* tempAttributeValue;
	char* buf;			//input 파일을 한줄씩 읽어오기 위한 버퍼
	char* tempBuf;		//buf에 저장된 문자들을 태그를 기준으로 자르기 위한 버퍼

	int idx;
	int startIdx;
	int endIdx;
	bool isRoute;
	bool isEmptyTag;

public:
	XMLNode* XpathRoute;
	bool checkAlpha(const char ch);
	bool checkNumber(const char ch);
	int checkChar(const char* str, const char _ch);
	bool checkByteOrderMark();
	int parser(const char* fileName, XMLNode* _XMLNode);
	void parserPI();
	void parserDTD();
	void parserComment();
	void parserStartTag();
	void parserEndTag();
	void parserContent();
	void parserAttribute(int _blankNum);
};

#endif /* XMLPARSER_H_ */
