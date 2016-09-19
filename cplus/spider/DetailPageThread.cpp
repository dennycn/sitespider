#include <kittylog/KittyLog.h>
#include "DetailPageThread.h"
#include "Popuse.h"
#include "TempletRead.h"
#include "DownloadConfig.h"

// ===  FUNCTION  ======================================================================
//  Name:  DetailPageThread
//  Description: It's used for text download
//  1 Get the text task from the textQueue
//  2 Download the text url and incode to gbk
//  3 Analysis the page content
//  4 Get the useful message from the content
//  5 Write the file
//  6 Put it into the SendQueue
//  7 DetailPageThreadEnd
// =====================================================================================
void DetailPageThread::start()
{
        IceUtil::Thread::start().detach();
}

void DetailPageThread::run()
{
        CPageData* taskValue = NULL;
        int ret = 0;

        KittyLog::debug("DetailPageThread start...");
        //while ( '@' == DownloadConfig::instance().getStopSignal() || DownloadConfig::instance().textQueueNum > 0 ) {
        while (1) {
                printf("\t\tget detailpage seed...\n");
                taskValue = NULL;
                // get detailpage url
                ret = DownloadConfig::instance().getTextTask(taskValue);
                if (ret == 1) {
                        sleep(3);
                        KittyLog::debug("AFTER GET TEXT TASK, textQueue.size() = 0");
                        if (taskValue != NULL) {
                                delete taskValue;
                                taskValue =NULL;
                                continue;
                        }
                }
                if (ret == -1 || taskValue == NULL) {
                        continue;
                }

                // download detailpage
                string filename = "";
                if ( fetch (taskValue, filename) )
                        continue;

                // parse detailpage
                if ( parsePage (taskValue, filename) )
                        continue;

        }
        KittyLog::debug("DetailPageThread end.");

        //DownloadConfig::instance().delDetailPageThreadNum();
}

// return: 0-ok; -1:error
int DetailPageThread::fetch(CPageData* &value, string& filename)
{
        if ( NULL == value )
                return -1;

        char command[1024];
        bzero(command, sizeof(command));
        string tmpUrlName = value->getTextUrl();
        if (tmpUrlName.size() < 12 || tmpUrlName.size() > 256) {
                KittyLog::debug("%s|%s", "Too long url is", tmpUrlName.c_str());
                if (value != NULL)
                        delete value;
                value = NULL;
                return -1;
        }
        //Note: test use origin url name
        filename = DownloadConfig::instance().makeFileName(tmpUrlName);
        //filename += tmpNowSec;
        printf("origin url = %s\n, save url=%s\n", tmpUrlName.c_str(), filename.c_str());
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

// parse detailpage
// return: 0－ok, -1:error
int DetailPageThread::parsePage(CPageData* &value, const string& filename)
{
        int ret;
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
        char* zhuanmaBuf;
        zhuanmaBuf = new char[1024*1024+1];
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

        // ues templet to parse detailpage: extract url
        ret = templetRead.ParseText(value, htmlbuf, zhuanmaBuf);
        if (ret <= -1) {
                KittyLog::debug("ParseText error: %d|%s", ret, value->getTextUrl());
                if (value != NULL)
                        delete value;
                value = NULL;
                return -1;
        }
        string tmpUrlName = value->getTextUrl();;
        if ((ret = DownloadConfig::instance().checkDownMsg(tmpUrlName)) != 0) {
                DownloadConfig::instance().writeFile(value);
                KittyLog::debug("Download url is %s\n, have store to xml", tmpUrlName.c_str());
                /*
                #if NOSENDFLG
                if (value != NULL)
                delete value;
                value = NULL;
                #endif

                #if SENDFLG
                // DownloadConfig::instance().setSendTask(taskValue);
                #endif
                */
        } 
        
        if (value != NULL)
                delete value;
        value = NULL;

        if (downBuf != NULL)
                delete[] downBuf;
        downBuf = NULL;
        
        if (zhuanmaBuf != NULL)
                delete[] zhuanmaBuf;
        zhuanmaBuf = NULL;
        //Note:
        //DownloadConfig::instance().delListPageThreadNum();

        return 0;
}

