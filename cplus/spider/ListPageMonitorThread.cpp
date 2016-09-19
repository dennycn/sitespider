#include <sys/stat.h>
#include <kittylog/KittyLog.h>
#include "ListPageMonitorThread.h"
#include "Popuse.h"
#include "TempletRead.h"
#include "DownloadConfig.h"

// ===  FUNCTION  ======================================================================
//  Name:  ListPageMonitorThread
//  Description:  It's used for title download , after download it produce text
//  1 Get the title task from the titleVector
//  2 Download the title url and incode to gbk
//  3 Analysis the page content
//  4 Get the text url from the content and paichong
//  5 Put the text url into textQueue
//  6 ListPageMonitorThreadEnd
// =====================================================================================
// @version: 1.0.0
// @brife: this version only one thread, to monitor listpagevector
ListPageMonitorThread::ListPageMonitorThread()
{
        titleVectorNum = 0;
        titleVecPos = 0;
        pthread_mutex_init(&titleLock, NULL);
        // template variable
        titleTempletNum = 0;

        md5UndownTree = new BigSearch<INT64>(1048576, 512, 8);
        pthread_mutex_init(&undownLock, NULL);

}

ListPageMonitorThread::~ListPageMonitorThread()
{
        pthread_mutex_destroy(&titleLock);

        // clean listPageVector
}

int ListPageMonitorThread::initSeed()
{       // listPage seed init
        string str;
        int ret;

        ifstream infile("conf/listpage.txt", ios::in);
        CPageData* tmpPopuse = NULL;
        while (getline(infile, str)) {
                StringMore::removeLastStr(str);
                // T: Title, X: text, S: vector
                if ('T' != str[0])
                        continue;

                tmpPopuse = new CPageData;
                // flgnum: tag nums
                int flgnum = 0;
                string tmpstr = str;
                string tmpStrValue;
                while (flgnum < 17) {
                        ret = (signed int)str.find("|");
                        tmpStrValue  =  str.substr(0, ret);
                        str = str.substr(ret+1, str.size()-ret);
                        switch ( flgnum ) {
                        case 0:
                                tmpPopuse->setProperty(tmpStrValue[0]);
                                flgnum++;
                                break;
                        case 1:
                                tmpPopuse->setDownloadId(atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 2:
                                tmpPopuse->setModifyType(tmpStrValue);
                                flgnum++;
                                break;
                        case 3:
                                tmpPopuse->setListUrl(tmpStrValue);
                                flgnum++;
                                break;
                        case 4:
                                tmpPopuse->setTitleUrl(tmpStrValue);
                                flgnum++;
                                break;
                        case 5:
                                tmpPopuse->setTextUrl(tmpStrValue);
                                flgnum++;
                                break;
                        case 6:
                                tmpPopuse->setListTemNum(atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 7:
                                tmpPopuse->setTitleTemNum(atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 8:
                                tmpPopuse->setTextTemNum(atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 9:
                                tmpPopuse->setPost(tmpStrValue);
                                flgnum++;
                                break;
                        case 10:
                                tmpPopuse->setPasswd(tmpStrValue);
                                flgnum++;
                                break;
                        case 11:
                                tmpPopuse->setLabel(tmpStrValue);
                                flgnum++;
                                break;
                        case 12:
                                tmpPopuse->setWebsite(tmpStrValue);
                                flgnum++;
                                break;
                        case 13:
                                tmpPopuse->setDownloadFlg(tmpStrValue[0]);
                                flgnum++;
                                break;
                        case 14:
                                tmpPopuse->setWriteFileFlg( atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 15:
                                tmpPopuse->setTranscode(atoi(tmpStrValue.c_str()));
                                flgnum++;
                                break;
                        case 16:
                                tmpPopuse->setImgUrl(tmpStrValue);
                                flgnum++;
                                break;
                        default:
                                break;
                        }				// -----  end switch  -----
                }//-------end while(flgnum < 17)-------------
                // continue parse data
                str = tmpstr;
                titleVectorNum++;
                titleVector.push_back(tmpPopuse);
                tmpPopuse = NULL;
        }// while (getline
        KittyLog::info("listPageVectorNum = %d", titleVectorNum);

        infile.close();
        return 0;
}

int ListPageMonitorThread::initTemplate()
{       // init listpage templet
        string str;
        int ret = 0;

        Templet_Data* tmpTemplet = NULL;
        Templet_Data* tmpTempletNext = NULL;
        ifstream infile("conf/TempletTitle.txt", ios::in);
        if ( !infile) {
                cout << "open file: " << endl;
                return -1;
        }
        while (getline(infile, str)) {
                StringMore::removeLastStr(str);
                if ('#' == str[0])
                        continue;
                // template start flag
                if (str.compare(0,7,"!$NUMB=") == 0) {
                        ret = 1;
                        str = str.substr(7, str.size()-7);
                        titleTempletNum++;
                        continue;
                }
                // template end flag
                if (str.compare(0, 7, "/$NUME=") == 0) {
                        continue;
                }
                if ((signed int)str.find("_b") > 0) {
                        if (ret == 1) {
                                ret = 0;
                                tmpTemplet = NULL;
                                tmpTemplet = new Templet_Data;
                                titleTemVec.push_back(tmpTemplet);
                                tmpTempletNext = tmpTemplet;
                        } else {
                                tmpTemplet = NULL;
                                tmpTemplet = new Templet_Data;
                                tmpTempletNext->setNext(tmpTemplet);
                                tmpTempletNext = tmpTemplet;
                        }
                }
                tmpTemplet->splitBuf(str);
        }
        infile.close();
        KittyLog::info("listpage TempletNum = %d", titleTempletNum);
        
        return 0;
}

void ListPageMonitorThread::start()
{
        initSeed();
        initTemplate();
        IceUtil::Thread::start().detach();
}

void ListPageMonitorThread::run()
{
        KittyLog::debug("ListPageMonitorThread start...");
        CPageData* taskValue = NULL;;
        int ret = 0;

        // '@' == DownloadConfig::instance().getStopSignal()
        while (1) {
                printf("\t\tget listpage seed[%d]...\n", titleVecPos);
                taskValue = NULL;
                // get listpage url
                ret = this->getTitleTask(taskValue);
                if (-1 == ret) {
                        continue;
                };

                // download listpage
                string filename = "";
                if ( fetch (taskValue, filename) )
                        continue;

                // parse listpage
                if ( parseListPage (taskValue, filename) )
                        continue;

        } // end while
}

// ===  FUNCTION  ======================================================================
//  Name:  getTitleTask
//  Description:  get the title task from titleVector
//  return: -1:error,
// =====================================================================================
int ListPageMonitorThread::getTitleTask (CPageData* &value)
{
        pthread_mutex_lock(&titleLock);
        CPageData* tmpValue = NULL;
        while (titleVecPos < titleVectorNum) {
                tmpValue = titleVector[titleVecPos];
                titleVecPos++;
                
                // use roll polling
                if (titleVecPos >= titleVectorNum) {
                         titleVecPos = 0; 
                }
                // frontier: only get valid url
                if (tmpValue != NULL && 'Y' == tmpValue->getDownloadFlg()) {
                        value = tmpValue;
                        break;
                }
        }
        pthread_mutex_unlock(&titleLock);

        if (value != NULL)
                return 0;
        else
                return -1;
}

// brief: download page
// return: 0-ok; -1:error
// Note: will call Fetcher::instance().fecth() instead of this
int ListPageMonitorThread::fetch(CPageData* &value, string& filename)
{
        //Fetcher::instance().fecth();
        if ( NULL == value )
                return -1;

        char command[1024];
        bzero(command, sizeof(command));
        Mytime  mytime;
        string tmpNowSec = mytime.getNowSec();
        string tmpUrlName = value->getTitleUrl();
        //Note: test use origin url name
        filename = DownloadConfig::instance().makeFileName(tmpUrlName);
        filename += tmpNowSec;
        printf("\t\torigin url = %s\n, \t\tsave url=%s\n", tmpUrlName.c_str(), filename.c_str());
        snprintf(command, 1023, "curl -m 120 -A \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)\" -o %s \"%s\" -s",
                 filename.c_str(), tmpUrlName.c_str());

        int iTryCount = 0;
        int ret;
        // try to download page
        while ( (ret = system(command)) != 0) {
                iTryCount++;
                sleep(5);
                if (iTryCount <=4)
                        continue;
                else
                        return -1;
        }

        return 0;
}

// brief: parse listpage
// return: 0－ok, -1:error
// Note: will call Extractor::instance().extract() instead of this
int ListPageMonitorThread::parseListPage(CPageData* &value, const string& filename)
{
        int ret;
        CPageData* tmpTask;
        char* downBuf = new char[1024*1024+1];
        TempletRead templetRead;
        string htmlbuf;
        struct stat statbuf;
        statbuf.st_size = 0;
        ret = stat(filename.c_str(), &statbuf);
        if (ret != 0 || statbuf.st_size <= 0 ) {
                remove(filename.c_str());
                return -1;
        }

        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == NULL) {
                return -1;
        }

        bzero(downBuf, 1024*1024);
        fread(downBuf, 1024*1024, 1,fp);
        fclose(fp);
        //remove(filename.c_str());

        //Charecter zhuanma;
        //char* zhuanmaBuf = new char[1024*1024+1];
        // convert page encode to the gb18030
        //if (value->getTranscode() == 1) {
        //            int inLen = strlen(downBuf);
        //            int outLen = 1024*1024;
        //            int flag;
        //            int tmpLen;
        //            tmpLen = zhuanma.iconv_local_to_gb18030(downBuf,inLen, zhuanmaBuf, outLen, flag );//
        //        htmlbuf = zhuanmaBuf;
        //} else {
        htmlbuf = downBuf;
        //}

        // ues templet to parse listpage: extract url
        ret = templetRead.ParseTitle(value, htmlbuf);
        KittyLog::debug("templetRead taskStoreSize =[%d]", templetRead.taskStore.size());      
        if (ret <= -1 || templetRead.taskStore.size()<= 0 ) {
                return -1;
        }

        string tmpUrlName;
        while (templetRead.taskStore.size()> 0) {
                //KittyLog::debug("TempletRead Store size is |%d|", templetRead.taskStore.size());
                tmpTask = NULL;
                tmpTask = templetRead.taskStore.front();
                templetRead.taskStore.pop();
                templetRead.taskNum = templetRead.taskStore.size();
                if (tmpTask == NULL)
                        continue;
                tmpUrlName = tmpTask->getTextUrl();
                if (tmpUrlName.size() <= 7 || tmpUrlName.size() > 256) {
                        if (tmpTask != NULL)
                                delete tmpTask;
                        tmpTask = NULL;
                        continue;
                }
                if ((ret = this->checkUndownMsg(tmpUrlName)) != 0) {
                        DownloadConfig::instance().setTextTask(tmpTask);
                } else {
                        if (tmpTask != NULL) {
                                delete tmpTask;
                                tmpTask = NULL;
                        }
                }
        }

        if (downBuf != NULL)
                delete[] downBuf;
        downBuf = NULL;
//        if (zhuanmaBuf != NULL)
//                delete[] zhuanmaBuf;
//        zhuanmaBuf = NULL;
        //Note:
        //DownloadConfig::instance().delListPageThreadNum();

        return 0;
}

// ===  FUNCTION  ======================================================================
//  Name:  getTitleTemplet
//  Description:  get the title templet
// =====================================================================================
Templet_Data* ListPageMonitorThread::getTitleTemplet (int tmpi)
{
        if (tmpi >= titleTempletNum)
                return NULL;
        else
                return titleTemVec[tmpi];
}

//  Name:  checkUndownMsg
//  Description:  check the text whether in the textQueue
//       return:  0 exist !0 not exist
// =====================================================================================
int ListPageMonitorThread::checkUndownMsg (const string& value)
{
        INT64 tmpMd5 = 0;
        void* tmpValue = NULL;
        void* tmpValueOld = NULL;
        int tmpflg = 0;
        int tmpNumIn = 1;
        MD5String(value.c_str(), (unsigned char*)&tmpMd5, 8);
        pthread_mutex_lock(&undownLock);
        tmpflg = md5UndownTree->Search(tmpMd5,tmpValue);
        if ( 0 != tmpflg && 0 != tmpMd5) {
                md5UndownTree->add(tmpMd5,&tmpNumIn,tmpValueOld);
        }
        pthread_mutex_unlock(&undownLock);
        return tmpflg;

}		

