// =====================================================================================
//
//       Filename:  DownloadConfig.h
//
//    Description:  Config of the ihandy_ycnews1.0
//
//        Version:  1.0
//        Created:  12/17/09 11:53:08
//       Revision:  none
//       Compiler:  g++
//
//         Author:  hanwei(the company of ihandy);, hanwei.rabbit@gmail.com
//        Company:  ihandy
//
// =====================================================================================
#ifndef _DOWNLOADCONFIG_H_
#define _DOWNLOADCONFIG_H_

#include "defines.h"
#include <time.h>
#include <vector>
#include <queue>
#include <sys/stat.h>
#include <thread/Thread.h>
#include <thread/Singleton.h>
#include <thread/Mutex.h>
#include <thread/Lock.h>
#include <thread/Exception.h>
#include "md5/md5.h"
#include "md5/md5int.h"
#include "md5/DataFind.h"
#include "Templet.h"
#include "Popuse.h"
#include "StringMore.h"

using namespace std;
using namespace StringMore;

class DownloadConfig :public IceUtil::Thread,
                        public MyUtil::Singleton<DownloadConfig>

{
public:
        DownloadConfig();
        ~DownloadConfig();
        virtual void run();

        void writeFile(CPageData* &tmpValue);
        int configInit();

        Templet_Data* getTextTemplet(int tmpi);
        string makeFileName(const string value);
        string  getDateName();//need to acculmulate

        char getStopSignal();
        
        void setTextTask(CPageData* &tmpValue);
        int getTextTask(CPageData* &tmpValue);
//        int md5LoadTree();
//        int md5SaveTree();//it need to use lock error:-1 success:0
        int checkDownMsg(const string& value);
private:
        void recyler();
        void configPathInit();
        string txtPath;
        string md5Path;
        string logPath;
        string savePath;
        string filePath;
public:
        string configName;
        string configChangeName;
        string md5DownName;
        string md5UnDownName;
        string dateName;//name is update by getDateName;
        string logName;//name is update everyday  error or not download is written in the logName
        string saveName;//name is update everyday save the download news file and will send for check at the 1 o'clock

        //Bigsearch<INT64> *md5SendTree;

        queue<CPageData*> textQueue;
        int textQueueNum;
//        queue<CPageData*> sendQueue;
//        int sendQueueNum;
//
        char Stop_signal;
        //int textThreadNum;
        //int titleThreadNum;
//        string sendIp;
//        bool IPchange;
//        pthread_mutex_t logLock;
//        pthread_mutex_t md5treeLock;
//        pthread_mutex_t vectorLock;
//        pthread_mutex_t textThreadNumLock;
//        pthread_mutex_t titleThreadNumLock;
//        pthread_mutex_t sendLock;
        pthread_mutex_t writeLock;
        pthread_mutex_t downLock;
        BigSearch<INT64> *md5DownTree;
//
// remove from here: deatilPage
        vector<Templet_Data*> textTemVec;
        pthread_mutex_t textLock;
        int textTempletNum;
        
        // next member will remove from here
public:
        //int configChange();
        //void setSendTask(CPageData* &tmpValue);
        //int getSendTask(CPageData* &tmpValue);
        //int getSendQueueNum();
        //string getDateNameTime();
        //int getListTask(CPageData* &value);
        //int getListPageThreadNum();
        //void delListPageThreadNum();
        //int getDetailPageThreadNum();
        //void delDetailPageThreadNum();
};

#endif

