#include "StringMore.h"

bool StringMore::checkChar(char valueChar)
{
        if (' ' == valueChar || '\t' == valueChar || '\n' == valueChar || '\r' == valueChar)
                return true;
        else
                return false;
}
/*
   funciotn name:removeLastStr
   use: ÓÃÓÚÉ¾³ý×Ö·û´®×îºóÒ»¸ö×Ö·ûÊÇ¿Õ¸ñ »Ø³µ »»ÐÐ tab
   return :·µ»Ø´¦ÀíºóµÄ×Ö·û´®³¤¶È
   */
int StringMore::removeLastStr(string& value)
{
        if (value.empty())
                return 0;
        assert(value.size() >= 0);
        int strLen = value.size();
        while (strLen > 0 &&
                        (' ' == value[strLen-1] || '\t' == value[strLen-1] || '\n' == value[strLen-1] || '\r' == value[strLen-1]) ) {
                value.erase(strLen-1,1);
                strLen--;
        }
        if (strLen >= 0)
                return strLen;
        else
                return 0;
}
/*
   function name: removeMiddleStr
   use: ÓÃÓÚÉ¾³ý×Ö·û´®ÖÐº¬ÓÐ×Ö·ûÊÇ¿Õ¸ñ »Ø³µ »»ÐÐ tab
   return :·µ»Ø´¦ÀíºóµÄ×Ö·û´®³¤¶È
   */
int StringMore::removeMiddleStr(string& value)
{
        int strLen = removeLastStr(value);
        assert(strLen >= 0);
        if (strLen == 0)
                return strLen;
        value.erase(remove_if(value.begin(),value.end(),checkChar),value.end());
        strLen = value.size();
        return strLen;
}
/*
 *funciton name:removeScript
 *use:ÓÃÓÚÉ¾³ýscriptÖ®¼äµÄÓï¾
 * */
int StringMore::removeScript(char*& text)
{
        char* p = text;
        int i = 0;
        while (*p) {
                if (*p < 0) {
                        text[i++] = *p++;
                        text[i++] = *p++;
                } else if (*p == '<') {
                        if ( strncasecmp(p, "<script", 7) == 0) {
                                p += 2;
                                while (*p && *p != '>')
                                        p++;
                                if (*p == '>')
                                        p++;
                                while (*p) {
                                        if (strncasecmp(p, "</script", 8) == 0) {
                                                p += 2;
                                                while (*p && *p != '>')
                                                        p++;
                                                if (*p == '>') {
                                                        p++;
                                                        break;
                                                }

                                        } else {
                                                p++;
                                        }
                                }
                        } else if (strncasecmp(p, "<style", 6) == 0) {
                                p += 2;
                                while (*p && *p != '>')
                                        p++;
                                if (*p == '>')
                                        p++;
                                while (*p) {
                                        if (strncasecmp(p, "</style", 7) == 0) {
                                                p += 2;
                                                while (*p && *p != '>')
                                                        p++;
                                                if (*p == '>') {
                                                        p++;
                                                        break;
                                                }

                                        } else {
                                                p++;
                                        }
                                }

                        } else {
                                text[i++] = *p++;
                        }
                } else
                        text[i++] = *p++;
        }
        text[i] = 0;
        text[i+1] = 0;
        i = strlen(text);
        return i;

}
int StringMore::removeScript(string& value)
{
        int i , j;
        while ( (i = (signed int)value.find("<SCRIPT")) >= 0) {
                j = (signed int)value.find("</SCRIPT>", i);
                if (j >= 0) {
                        value = value.replace(i, j+9-i, " ");
                }
        }
        while ( (i = (signed int)value.find("<script")) >= 0) {
                j = (signed int)value.find("</script>", i);
                if (j >= 0) {
                        value = value.replace(i, j+9-i, " ");
                }
        }
        j = value.size();
        return j;
}
/*
 *function name:removeAlong
 *use: ÓÃÓÚÉ¾³ý<a <font <!-- -->
 * */
int StringMore::removeAlong(char*& text)
{
        char* p = text;
        int i =0;
        i = removeScript(text);
        i = 0;
        while (*p) {
                if (*p < 0) {
                        text[i++] = *p++;
                        text[i++] = *p++;
                } else if (*p == '<') {
                        if ( strncasecmp(p, "<t", 2) == 0 || strncasecmp(p, "<c", 2) == 0 ||
                                        strncasecmp(p, "<f", 2) == 0 || strncasecmp(p, "<s", 2) == 0 ||
                                        strncasecmp(p, "<a", 2) == 0 || strncasecmp(p, "<d", 2) == 0 ||
                                        strncasecmp(p, "<o", 2) == 0 || strncasecmp(p, "<e", 2) == 0 ||
                                        strncasecmp(p, "<h", 2) == 0 || strncasecmp(p, "<!", 2) == 0 ||
                                        strncasecmp(p, "<i", 2) == 0 || strncasecmp(p, "<l", 2) == 0 ||
                                        strncasecmp(p, "</t",3) == 0 || strncasecmp(p, "</c",3) == 0 ||
                                        strncasecmp(p, "</f",3) == 0 || strncasecmp(p, "</s",3) == 0 ||
                                        strncasecmp(p, "</a",3) == 0 || strncasecmp(p, "</d",3) == 0 ||
                                        strncasecmp(p, "</i",3) == 0 || strncasecmp(p, "</l",3) == 0 ||
                                        strncasecmp(p, "</o",3) == 0 || strncasecmp(p, "</e",3) == 0 ||
                                        strncasecmp(p, "</h",3) == 0 || strncasecmp(p, "<img", 4) == 0) {
                                p += 2;
                                while (*p && *p != '>')
                                        p++;
                                if (*p == '>')
                                        p++;
                        } else {
                                text[i++] = *p++;
                        }
                } else
                        text[i++] = *p++;
        }
        text[i] = 0;
        text[i+1] = 0;
        i = strlen(text);
        return i;
}
int StringMore::removeHtml(string& value)
{
        int i , j;
        i = removeSpecial(value);
        i = removeScript(value);
        while ((i = (signed int)value.find("{")) >= 0) {
                j = (signed int)value.find("}", i);
                value.replace(i, j-i+1, " ");
        }
        while ((i = (signed int)value.find("[")) >= 0) {
                j = (signed int)value.find("]", i);
                value.replace(i, j-i+1, " ");
        }

        while ( (i = (signed int)value.find("<")) >= 0) {
                j = (signed int)value.find(">",i);
                value.replace(i,j-i+1," ");
        }
        i = removeMiddleStr(value);
        j = value.size();
        return j;

}

/*
 * funciton removeSpecial
 * use: ÓÃÓÚÉ¾³ýÒ»Ð©ÌØÊâ×Ö·ûÈç&nbsp;=
 */
int StringMore::removeSpecial(string& value)
{
        int i;
        while ((i = (signed int)value.find("&nbsp;")) >= 0) {
                value.replace(i,6," ");
        }
        while ( (i = (signed int)value.find("&lt;")) >= 0) {
                value.replace(i, 4, "<");
        }
        while ( (i = (signed int)value.find("&gt;")) >= 0) {
                value.replace(i, 4, ">");
        }
        while ( (i = (signed int)value.find("&amp;")) >= 0) {
                value.replace(i, 5, "&");
        }
        while ( (i = (signed int)value.find("&quot;")) >= 0) {
                value.replace(i, 6, "\"");
        }
        while ( (i = (signed int)value.find("&middot;")) >= 0) {
                value.replace(i, 8, ".");
        }
        while ( (i = (signed int)value.find("&copy;")) >= 0) {
                value.replace(i, 6, " ");
        }
        while ( (i = (signed int)value.find("&reg;")) >= 0) {
                value.replace(i, 5, "@");
        }
        while ((i = (signed int)value.find("&ldquo;")) >= 0) {
                value.replace(i, 7, "\"");
        }
        while ((i = (signed int)value.find("&rdquo;")) >=0 ) {
                value.replace(i, 7, "\"");
        }
        while ((i = (signed int)value.find("&lsquo;")) >=0 ) {
                value.replace(i, 7, "'");
        }
        while ((i = (signed int)value.find("&rsquo;")) >=0 ) {
                value.replace(i, 7, "'");
        }
        /////////////////////////////////////////////////
        while ((i = (signed int)value.find("&nbsp")) >= 0) {
                value.replace(i,5," ");
                i = (signed int)value.find("&nbsp");
        }
        while ( (i = (signed int)value.find("&lt")) >= 0) {
                value.replace(i, 3, "<");
        }
        while ( (i = (signed int)value.find("&gt")) >= 0) {
                value.replace(i, 3, ">");
        }
        while ( (i = (signed int)value.find("&amp")) >= 0) {
                value.replace(i, 4, "&");
        }
        while ( (i = (signed int)value.find("&quot")) >= 0) {
                value.replace(i, 5, "\"");
        }
        while ( (i = (signed int)value.find("&middot")) >= 0) {
                value.replace(i, 7, ".");
        }
        while ( (i = (signed int)value.find("&copy")) >= 0) {
                value.replace(i, 5, " ");
        }
        while ( (i = (signed int)value.find("&reg")) >= 0) {
                value.replace(i, 4, "@");
        }
        while ((i = (signed int)value.find("&ldquo")) >= 0) {
                value.replace(i, 6, "\"");
        }
        while ((i = (signed int)value.find("&rdquo")) >=0 ) {
                value.replace(i, 6, "\"");
        }
        while ((i = (signed int)value.find("&lsquo")) >=0 ) {
                value.replace(i, 6, "'");
        }
        while ((i = (signed int)value.find("&rsquo")) >=0 ) {
                value.replace(i, 6, "'");
        }

        i = value.size();
        return i;
}
int StringMore::removeLink(char*& text)
{
        char* p = text;
        char* q = NULL;
        int i = 0;
        while (*p) {
                if (*p < 0) {
                        text[i++] = *p++;
                        text[i++] = *p++;
                } else if (*p == '<') {
                        if ( strncasecmp(p, "<a href", 7) == 0) {
                                p += 2;
                                while (*p && *p != '>')
                                        p++;
                                if (*p == '>')
                                        p++;
                                q = p;
                                while (*p) {
                                        if (strncasecmp(p, "</a", 3) == 0) {

                                                bool flag = true;
                                                char* q1 = q;
                                                for (; q<p-3; q++) {
                                                        if ((char)(0xBD) == *q && (char)(0xF8) == *(q+1)) {
                                                                if ( (char)(0xC8) == *(q+2) && (char)(0xEB) == *(q+3)) {
                                                                        flag = false;
                                                                        break;
                                                                }
                                                        }
                                                        if ((char)(0xB5) == *q && (char)(0xE3) == *(q+1)) {
                                                                if ( (char)(0xBB) == *(q+2) && (char)(0xF7) == *(q+3)) {
                                                                        flag = false;
                                                                        break;
                                                                }
                                                        }
                                                        if ((char)(0xB2) == *q && (char)(0xE9) == *(q+1)) {
                                                                if ( (char)(0xBF) == *(q+2) && (char)(0xB4) == *(q+3)) {
                                                                        flag = false;
                                                                        break;
                                                                }
                                                        }
                                                        if ((char)(0xB8) == *q && (char)(0xFC) == *(q+1)) {
                                                                if ( (char)(0xB6) == *(q+2) && (char)(0xE0) == *(q+3)) {
                                                                        flag = false;
                                                                        break;
                                                                }
                                                        }
                                                        if ( (char)(0xD2) == *q && (char)(0xB3) == *(q+1)) { //Ò³
                                                                flag = false;
                                                                break;
                                                        }

                                                }
                                                if (flag == true) {
                                                        q = q1;
                                                        int count = 0;
                                                        while (q<p) {
                                                                if (*q <= 0) {
                                                                        count++;
                                                                        q++;
                                                                }
                                                                q++;
                                                        }
                                                        if (count >= 6)
                                                                flag = false;
                                                }
                                                if (flag)
                                                        while ((p-q1) > 0) {
                                                                text[i++] = *q1++;
                                                        }

                                                p += 2;
                                                while (*p && *p != '>')
                                                        p++;
                                                if (*p == '>') {
                                                        p++;
                                                        break;
                                                }

                                        } else {
                                                p++;
                                        }
                                }
                        } else {
                                text[i++] = *p++;
                        }
                } else
                        text[i++] = *p++;
        }
        text[i] = 0;
        text[i+1] = 0;
        i = strlen(text);
        return i;


}

#if  0     // ----- #if 0 : If0Label_1 ----- 

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
using namespace std;
using namespace StringMore;
int main ( int argc, char *argv[] )
{
        struct stat statbuf;
        char* buf = NULL;
        char* beg, *end;
        char* text = new char[1024*1024];
        int flg;
        FILE* fp = NULL;
        flg = stat("dat", &statbuf);
        if (flg < 0)
                return -1;
        buf = new char[statbuf.st_size];
        bzero(buf, statbuf.st_size);
        fp = fopen("dat", "r");
        fread(buf, statbuf.st_size, 1, fp);
        fclose(fp);
        fp = NULL;
        char name[100];
        beg = buf;
        int num = 0;
        char* strbuf = buf;
        string tmpstr = buf;
        StringMore::removeSpecial(tmpstr);
        strncpy(strbuf, tmpstr.c_str(), tmpstr.size());
        StringMore::removeLink(strbuf);
        tmpstr = strbuf;
        cout<<"----------------------Html-----------------------"<<endl;
        cout<<tmpstr<<endl;
        cout<<"----------------------Html-----------------------"<<endl;
//	taskData->setTextHtml(tmpstr);
        StringMore::removeAlong(strbuf);
        cout<<"----------------------HtmlAlogn-----------------------"<<endl;
        cout<<strbuf<<endl;
        cout<<"----------------------HtmlAlogn-----------------------"<<endl;

        tmpstr = strbuf;
        StringMore::removeHtml(tmpstr);
        cout<<"----------------------Text-----------------------"<<endl;
        cout<<tmpstr<<endl;

        //while( (end = strstr(beg,"<TextHtml><![CDATA[")) != NULL)
        //{
        //	end += strlen("<TextHtml><![CDATA[");
        //	beg = end;
        //	end = strstr(beg, "]]></TextHtml>");
        //	bzero(text, 1024*1024);
        //	strncpy(text, beg, end-beg);
        //	bzero(name, sizeof(name));
        //	sprintf(name, "mirrors/%d.html", num++);
        //	fp = NULL;
        //	fp = fopen(name, "w");
        //	StringMore::removeAlong(text);
        //	fprintf(fp, "%s", text);
        //	fclose(fp);
        //	fp = NULL;
        //}
        return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
#endif     // ----- #if 0 : If0Label_1 ----- 

