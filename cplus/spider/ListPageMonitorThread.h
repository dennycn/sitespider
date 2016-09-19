#ifndef _TITLEMONITOR_THREAD_H
#define _TITLEMONITOR_THREAD_H

#include "defines.h"
#include <thread/Thread.h>
#include <thread/Singleton.h>
#include <thread/Mutex.h>
#include <thread/Lock.h>
#include <thread/Exception.h>
#include "md5/md5.h"
#include "md5/DataFind.h"
#include "Templet.h"
#include "Popuse.h"


class ListPageMonitorThread : public IceUtil::Thread,
                        public MyUtil::Singleton<ListPageMonitorThread>
{
public:
        ListPageMonitorThread();
        ~ListPageMonitorThread();
        void start();
        virtual void run();
        Templet_Data* getTitleTemplet(int tmpi);

private:
        int initSeed();
        int initTemplate();
        int fetch(CPageData* &value, string& filename);
        int parseListPage(CPageData* &value, const string& filename);
        int getTitleTask(CPageData* &value );
        char* getTitleUrl();
        int checkUndownMsg(const string& value);
        
private:
        // listpage seed
        pthread_mutex_t titleLock;
        vector<CPageData*> titleVector;
        int titleVectorNum;
        int titleVecPos;
        //char titleUrl[URLLEN];

        // listpage templet
        vector<Templet_Data*> titleTemVec;
        int titleTempletNum;

        // md5(url)  
        BigSearch<INT64> *md5UndownTree;
        pthread_mutex_t undownLock;
};


#endif
