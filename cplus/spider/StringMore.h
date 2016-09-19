#ifndef _STRINGMORE_H_
#define _STRINGMORE_H_
/*
   ���ļ���Ҫ���ڴ����ַ�����غ���
author:hanwei.rabbit@gmail.com
date:20091126 16:28
   */
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <strings.h>
#include <cassert>
#include <algorithm>
using namespace std;
namespace StringMore
{
/*
 * �����ж��ַ����Ƿ���� �ո� \t \n \r
 * */
extern bool checkChar(char valueChar);
/*{
    if(' ' == valueChar || '\t' == valueChar || '\n' == valueChar || '\r' == valueChar)
	return true;
    else
	return false;
}*/
/*
   funciotn name:removeLastStr
   use: ����ɾ���ַ������һ���ַ��ǿո� �س� ���� tab
   return :���ش������ַ�������
   */
extern int removeLastStr(string& value);
/*{
    if(value.empty())
	return 0;
    assert(value.size() >= 0);
    int strLen = value.size();
    while(strLen > 0 &&
	    (' ' == value[strLen-1] || '\t' == value[strLen-1] || '\n' == value[strLen-1] || '\r' == value[strLen-1]) )
    {
	value.erase(strLen-1,1);
	strLen--;
    }
    if(strLen >= 0)
	return strLen;
    else
	return 0;
}*/
/*
   function name: removeMiddleStr
   use: ����ɾ���ַ����к����ַ��ǿո� �س� ���� tab
   return :���ش������ַ�������
   */
extern int removeMiddleStr(string& value);
/*{
    int strLen = removeLastStr(value);
    assert(strLen >= 0);
    if(strLen == 0)
	return strLen;
    value.erase(remove_if(value.begin(),value.end(),checkChar),value.end());
    strLen = value.size();
    return strLen;
}*/
extern int removeScript(string& value);
extern int removeScript(char*& text);
extern int removeAlong(char*& text);
extern int removeHtml(string& value);
extern int removeLink(char*& text);
/*{
    int i , j;
    while( (i = (signed int)value.find("<")) > 0)
    {
	j = (signed int)value.find(">",i);
	value.replace(i,j," ");
    }
    removeMiddleStr(value);
}*/
extern int removeSpecial(string& value);
}
#endif
