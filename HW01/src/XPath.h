/*
 * XPath.h
 *
 *  Created on: 2014. 4. 7.
 *      Author: 신동환
 */

#include <stdlib.h>
#include <list>
#include <string>
#include <queue>
#include "XMLNode.h"

#ifndef XPATH_H_
#define XPATH_H_

#define MAX_CHAR_SIZE 500
#define MAX_BUF_SIZE 800

enum CommandType { search_TagName, search_AttributeName};

class XPath {
public:
	XPath();
	virtual ~XPath();

private:
	std::list<XMLNode*> nodeList;
	std::queue<XMLNode*> searchNodeQ;
	PrintType printType;

	bool firstCallSearch_All;

public:
	bool checkAlpha(const char ch);
	bool checkNumber(const char ch);

	int XPathCmdParser(char* _cmdBuf, XMLNode* _XpathRoute);//cmd분석 함수.
	void Search_All_NonString(XMLNode* _XpathRoute);
	void Search_All(XMLNode* _XpathRoute);
	void Search_All(XMLNode* _XpathRoute, const char* str, CommandType _commandType);//가장 기본적인 탐색함수. 전체 순회 방식
	void Search_Child(const char* str);
	void AddNodeList(XMLNode* node);	//노드를 노드 리스트에 추가하는 함수.
	void ClearNodeList();				//현재 위치와 상관없는 탐색의 경우, 탐색 시작전에 노드를 비우도록 함.
	void PrintNodeList(PrintType _type);//노드 리스트 전체를 출력할 때 사용.
	void PrintNodeQ();//searchNodeQ 에 있는 노드들 출력
};

#endif /* XPATH_H_ */
