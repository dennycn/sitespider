#include <kittylog/KittyLog.h>
#include "TempletRead.h"
#include "ListPageMonitorThread.h"

TempletRead::TempletRead()
{
        taskNum = 0;
}
TempletRead::~TempletRead()
{

}

// ===  FUNCTION  ======================================================================
//  Name:  Parse //text
//  Description:  used for text parse
// return: －1－input error, -2--no templet, -3-parse error
// =====================================================================================
int TempletRead::ParseText(CPageData* &taskData, string& htmlStr, char*& strbuf)
{
        if (taskData == NULL)
                return -1;
        Templet_Data* templetData = NULL;
        templetData = DownloadConfig::instance().getTextTemplet(taskData->getTextTemNum());
        if (templetData == NULL) {
                //no templet
                return -2;
        }
        string tmpbuf = htmlStr;
        string tmpstr;
        int tmpnext = 0;
        while (templetData != NULL) {
                if (templetData->compareID("list") == 1) {
                        tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                        if (tmpnext > 0)
                                tmpbuf =  tmpstr;
                } else if (templetData->compareID("text") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                        if (-1 == tmpnext) {
                                return -3;
                        }
                        tmpbuf = tmpbuf.substr(tmpnext,tmpbuf.size() - tmpnext);
                        StringMore::removeSpecial(tmpstr);
                        strncpy(strbuf, tmpstr.c_str(), 1048570);
                        StringMore::removeLink(strbuf);
                        tmpstr = strbuf;
                        taskData->setTextHtml(tmpstr);
                        StringMore::removeAlong(strbuf);
                        tmpstr = strbuf;
                        StringMore::removeHtml(tmpstr);
                        taskData->setText(tmpstr);
                } else if (templetData->compareID("date") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                        string fixed = templetData->getFixed();
                        if (-1 == tmpnext) {
                                string tmpstr2 = "";
                                taskData->setDate(tmpstr2, fixed);
                                templetData = templetData->getNext();
                                continue;
                        }
                        tmpbuf = tmpbuf.substr(tmpnext,tmpbuf.size() - tmpnext);
                        StringMore::removeSpecial(tmpstr);
                        taskData->setDate(tmpstr, fixed);
                } else if (templetData->compareID("media") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                        if (-1 == tmpnext) {
                                templetData = templetData->getNext();
                                continue;
                        }
                        tmpbuf = tmpbuf.substr(tmpnext,tmpbuf.size() - tmpnext);
                        taskData->setMedia(tmpstr);
                } else if (templetData->compareID("title") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                        if (-1 == tmpnext) {
                                return -3;
                        }
                        tmpbuf = tmpbuf.substr(tmpnext,tmpbuf.size() - tmpnext);
                        StringMore::removeSpecial(tmpstr);
                        StringMore::removeMiddleStr(tmpstr);
                        taskData->setTitle(tmpstr);
                } else if (templetData->compareID("MediaUrl") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf, tmpnext);
                        if (-1 == tmpnext) {
                                templetData = templetData->getNext();
                                continue;
                        }
                        tmpbuf = tmpbuf.substr(tmpnext, tmpbuf.size() - tmpnext);
                        taskData->setMediaUrl(tmpstr);
                } else if (templetData->compareID("go") == 1) {
                        tmpstr = tmpbuf;
                        tmpstr = templetData->getBuf(tmpbuf, tmpnext);
                        if (tmpnext >=  0 ) {
                                tmpbuf = tmpbuf.substr(tmpnext, tmpbuf.size() - tmpnext);
                        }
                }
                templetData = templetData->getNext();
        }
        return 0;
}

int TempletRead::ParseTitle(CPageData* &taskData, string& htmlStr)
{
        if (taskData == NULL)
                return -1;
        Templet_Data* templetData = NULL;
        Templet_Data* templetData2 = NULL;
        CPageData* taskData1 = NULL;
        templetData = ListPageMonitorThread::instance().getTitleTemplet(taskData->getTitleTemNum());
        KittyLog::debug(" current template = %d", taskData->getTitleTemNum());
        if (templetData == NULL) {
                return -2;
        }
        string tmpbuf = htmlStr;
        string tmpstr, tmpstr2;
        int tmpnext = 0;
        taskNum = 0;

        while (templetData != NULL) {
                printf("\t\tstrid = %s\n", templetData->_strid.c_str());
                if (templetData->compareID("nrec") == 1) {
                        tmpnext = 0;
                        while (tmpnext != -1) {
                                tmpstr = tmpbuf;
                                tmpstr = templetData->getBuf(tmpbuf,tmpnext);
                                if (tmpnext == -1)
                                        break;
                                tmpbuf = tmpbuf.substr(tmpnext,tmpbuf.size()-tmpnext);
                                taskData1 = NULL;
                                taskData1 = new CPageData;
                                taskData->makeNewsson(taskData1);
                                templetData2 = templetData->getNext();
                                if (tmpstr.empty() || tmpstr.size() <= 0)
                                        break;
                                while (templetData2 != NULL) {
                                        if (templetData2->compareID("nurl") == 1) {
                                                tmpstr2 = tmpstr;
                                                tmpstr2 =templetData2->getBuf(tmpstr,tmpnext);
                                                if (-1 == tmpnext) {
                                                        if (taskData1 != NULL)
                                                                delete taskData1;
                                                        taskData1 = NULL;
                                                        tmpnext = 0;
                                                        break;
                                                }
                                                tmpstr = tmpstr.substr(tmpnext,tmpstr.size() - tmpnext);
                                                if (templetData2->getFixed().size() > 0) {
                                                        string fixedStr = templetData2->getFixed();
                                                        if ((signed int)fixedStr.find("DOT") >= 0) {
                                                                fixedStr = tmpstr2;
                                                        }
                                                        string fatherUrl = taskData1->getTitleUrl();
                                                        int ret =getFormatUrl(fixedStr, fatherUrl);
                                                        if (ret == 1) {
                                                                tmpstr2 = fixedStr + tmpstr2;
                                                        } else if (ret == 2) {
                                                                tmpstr2 = fixedStr;
                                                        }
                                                }
                                                if (tmpstr2.size() < 12 || tmpstr2.empty() || tmpstr2.size() > 256) {
                                                        if (taskData1 != NULL) {
                                                                delete taskData1;
                                                                taskData1 = NULL;
                                                        }
                                                        tmpnext = 0;
                                                        break;
                                                }
                                                taskData1->setTextUrl(tmpstr2);
                                        } else if (templetData2->compareID("title") == 1) {
                                                tmpstr2 = tmpstr;
                                                tmpstr2 = templetData2->getBuf(tmpstr,tmpnext);
                                                if (-1 == tmpnext) {
                                                        if (taskData1 != NULL) {
                                                                delete taskData1;
                                                                taskData1 = NULL;
                                                        }
                                                        tmpnext = 0;
                                                        break;
                                                }
                                                tmpstr = tmpstr.substr(tmpnext,tmpstr.size() - tmpnext);
                                                StringMore::removeMiddleStr(tmpstr2);
                                                StringMore::removeSpecial(tmpstr2);
                                                taskData1->setTitle(tmpstr2);
                                        } else if (templetData2->compareID("date") ==1) {
                                                tmpstr2 = tmpstr;
                                                tmpstr2 = templetData2->getBuf(tmpstr,tmpnext);
                                                string tmpDateStr;
                                                if (templetData2->getFixed().size() > 0)
                                                        tmpDateStr = templetData2->getFixed();
                                                if (-1 == tmpnext) {
                                                        tmpstr2 = "";
                                                        taskData1->setDate(tmpstr2,tmpDateStr );
                                                        templetData2 = templetData2->getNext();
                                                        continue;
                                                }
                                                tmpstr = tmpstr.substr(tmpnext,tmpstr.size() - tmpnext);
                                                taskData1->setDate(tmpstr2, tmpDateStr);
                                        } else if (templetData2->compareID("go") == 1) {
                                                tmpstr2 = tmpstr;
                                                tmpstr2 = templetData2->getBuf(tmpstr, tmpnext);
                                                if (tmpnext >= 0) {
                                                        tmpstr = tmpstr.substr(tmpnext, tmpstr.size()- tmpnext);
                                                }
                                        }
                                        templetData2 = templetData2->getNext();
                                }
                                if (taskData1 != NULL) {
                                        taskStore.push(taskData1);
                                        taskNum++;
                                }
                        }
                        return 0;
                } else if (templetData->compareID("list") == 1) {
                        tmpbuf = templetData->getBuf(tmpbuf,tmpnext);
                } else if (templetData->compareID("go") == 1) {
                        tmpstr = templetData->getBuf(tmpbuf, tmpnext);
                        if (tmpnext > 0) {
                                tmpbuf = tmpbuf.substr(tmpnext, tmpbuf.size() - tmpnext);
                        }
                }
                templetData = templetData->getNext();
        }
        return 0;
}
int TempletRead::getFormatUrl(string& fixedStr, string& fatherUrl)
{
        if ((signed int)fixedStr.find("SUBURL") >= 0) {
                int fatherflg;
                fatherflg = (signed int)fatherUrl.find("http://");
                if (fatherflg >= 0) {
                        fatherflg = (signed int)fatherUrl.find("/", 7);
                        if (fatherflg > 0) {
                                fatherUrl = fatherUrl.substr(0, fatherflg+1);
                        }
                }
                fixedStr = fatherUrl;
                return 1;
        } else if ( (signed int)fixedStr.find("../../../") >= 0) {
                fixedStr = fixedStr.substr(0, fixedStr.size() - 9);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len+1);
                fixedStr = tmpurl + fixedStr;
                return 2;
        } else if ((signed int)fixedStr.find("../../") >= 0) {
                fixedStr = fixedStr.substr(6, fixedStr.size() - 6);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len+1);
                fixedStr = tmpurl + fixedStr;
                return 2;
        } else if ((signed int)fixedStr.find("../") >= 0) {
                fixedStr = fixedStr.substr(3, fixedStr.size() - 3);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len+1);
                fixedStr = tmpurl + fixedStr;
                return 2;
        } else if ((signed int)fixedStr.find("./") >= 0) {
                fixedStr = fixedStr.substr(2, fixedStr.size()-2);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                fixedStr = tmpurl + fixedStr;
                return 2;
        } else {
                return 1;
        }
}

#if  0     // ----- #if 0 : If0Label_1 -----  test for getFormatUrl
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
/**
 * @brief
 *
 */
int main ( int argc, char *argv[] )
{
        string fatherUrl = "http://www.caihuanet.com/hsstock/news/pingji/";
        string fixedStr = "./201001/t20100129_1377390.shtml";
        if ((signed int)fixedStr.find("../") >= 0) {
                fixedStr = fixedStr.substr(3, fixedStr.size() - 3);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len);
                len = (signed int)tmpurl.find_last_of("/");
                tmpurl = tmpurl.substr(0, len+1);
                fixedStr = tmpurl + fixedStr;
                cout<<fixedStr<<endl;;
        } else if ((signed int)fixedStr.find("./") >= 0) {
                fixedStr = fixedStr.substr(2, fixedStr.size() - 2);
                string tmpurl = fatherUrl;
                int len = tmpurl.size();
                if (tmpurl[len-1] != '/') {
                        tmpurl = tmpurl + "/";
                }
                fixedStr = tmpurl  + fixedStr ;
                cout<<fixedStr<<endl;;
        }

        return EXIT_SUCCESS;
}
#endif     // ----- #if 0 : If0Label_1 ----- 

