// =====================================================================================
//
//       Filename:  Popuse.h
//
//    Description:  used for download the pop page
//
//        Version:  1.0
//        Created:  12/17/09 14:58:30
//       Revision:  none
//       Compiler:  g++
//
//         Author:  hanwei (the company of ihandy), hanwei.rabbit@gmail.com
//        Company:  ihandy
//
// =====================================================================================
#ifndef  _POPUSE_H_
#define  _POPUSE_H_
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "Templet.h"
#include "MyTime.h"
#include "StringMore.h"
#define	URLLEN 256			//
using namespace std;
using namespace StringMore;

// =====================================================================================
//        Class:  CPageData
//  Description:  pop templet web page
// =====================================================================================
class CPageData
{
public:
        CPageData () {
                downloadId = -1;
                listTemNum = -1;
                textTemNum = -1;
                titlePos = -1;
                property = 'A';
                downloadFlg = 'Y';
                writeFileFlg = 0;
                transcode = 1;
                bzero(listUrl, URLLEN);
                bzero(textUrl, URLLEN);
                bzero(titleUrl, URLLEN);
        }
        ~CPageData() {
                //		if(listUrl != NULL)
                //			delete[] listUrl;
                //		listUrl = NULL;
                //		if(textUrl != NULL)
                //			delete[] textUrl;
                //		textUrl = NULL;
                //		if(titleUrl != NULL)
                //			delete[] titleUrl;
                //		titleUrl = NULL;
        }
        void setText(string tmpvalue);
        string getText();
        string getTitle();
        void setTextHtml(string tmpvalue);
        void  setDate(string tmpvalue, string tmpstrFlg);
        string  getDate ();
        void setMedia (string tmpvalue);
        string getMedia ();
        void setMediaUrl(string tmpvalue);
        string getMediaUrl ();
        void setTitle (string tmpvalue);
        void setTextUrl (string tmpvalue);
        char* getTextUrl();
        //Note:
        char* getTitleUrl();


        string getTextHtml();
        int makeNewsson(CPageData* taskData);
        int getTitleTemNum();
        int getTextTemNum();
        void setTitleUrl(string tmpvalue);
        void setListUrl(string tmpvalue);
        void setModifyType(string tmpvalue);
        void setDownloadId(int tmpvalue);
        void setProperty(char tmpvalue);
        void setListTemNum(int tmpvalue);
        void setTitleTemNum(int tmpvalue);
        void setTextTemNum(int tmpvalue);
        void setPost(string tmpvalue);
        void setPasswd(string tmpvalue);
        void setLabel(string tmpvalue);
        void setWebsite(string tmpvalue);
        void setDownloadFlg(char tmpvalue);
        void setWriteFileFlg(int tmpvalue);
        void setTranscode(int tmpvalue);
        int getTranscode();
        void setImgUrl(string tmpvalue);
        int getDownloadId();
        char getDownloadFlg();
        int getNow();
        string getPublishTime();
        string getLabel();
        string getWebsite();
        // ====================  DATA MEMBERS  =======================================
private:
        int downloadId;
        string modifyType;//ADD DEL MOD
        char listUrl[URLLEN];
        char textUrl[URLLEN];
        char titleUrl[URLLEN];
//	string listUrl;
//	string textUrl;
//	string titleUrl;
        int listTemNum;
        int textTemNum;
        int titlePos;
        string post;//...
        string passwd;//123456
        string label;//国际财经
        string website;//sina hexun
        char property;// text title list === x t s
        char downloadFlg;//Y or N
        int writeFileFlg;
        int transcode;
        string imgUrl;
        string videoUrl;
        int now;//time int
        string publishTime;
        string title;
        string text;
        string media;
        string mediaUrl;
        string textHtml;
        string nextPage;
        char nextPageFlg;

}; // -----  end of class CPageData  -----

#endif   // ----- #ifndef _POPUSE_H_  -----



