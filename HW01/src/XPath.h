/*
 * XPath.h
 *
 *  Created on: 2014. 4. 7.
 *      Author: 신동환
 */

#include <list>
#include <string>
#include "XMLNode.h"

enum commandType { search_TagName, search_Attribute};

#ifndef XPATH_H_
#define XPATH_H_

class XPath {
public:
	XPath();
	virtual ~XPath();

private:
	std::list<XMLNode*> nodeList;

public:
	int XPathCmdParser();				//cmd분석 함수.
	void Search_All(XMLNode* _XpathRoute, const char* str, commandType _commandType);//가장 기본적인 탐색함수. 전체 순회 방식
	void AddNodeList(XMLNode* node);	//노드를 노드 리스트에 추가하는 함수.
	void ClearNodeList();				//현재 위치와 상관없는 탐색의 경우, 탐색 시작전에 노드를 비우도록 함.
	void PrintNode();					//특정 노드만 출력 시킬때 사용. XMLNode class로 이동시켜야 할수도.
	void PrintNodeList();				//노드 리스트 전체를 출력할 때 사용.
};

#endif /* XPATH_H_ */
