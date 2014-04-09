/*
 * XString.cpp
 *
 *  Created on: 2014. 4. 9.
 *      Author: 신동환
 */

#include "XString.h"

XString::XString() {
}

XString::~XString() {
}

//ch가 알파벳인지 검사하는 함수
bool XString::checkAlpha(const char ch)
{
	if ('a' <= ch && ch <= 'z') return true;
	else if ('A' <= ch && ch <= 'Z') return true;
	else return false;
}

//ch가 숫자인지 검사하는 함수
bool XString::checkNumber(const char ch)
{
	if ('0' <= ch && ch <= '9') return true;
	else return false;
}

//문자열str 에서 _ch문자가 있는지 검사하는 함수. 문자열 처음부터 _last 문자가 나타날때 까지 루프를 반복함.
int XString::checkAnyChar(const char* str, const char _ch, const char _last)
{
	int _idx = 0;
	while (str[_idx] != _last)
	{
		if(str[_idx] == _ch) return _idx;
		_idx++;
	}

	return -1;
}

//공백제거
void XString::RemoveBlank(char* _str, int* _idx)
{
	while(_str[*_idx] == ' ' || _str[*_idx] == '\t') (*_idx)++;
}
