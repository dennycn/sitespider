#ifndef _TEXT_THREAD_H
#define _TEXT_THREAD_H

#include <thread/Thread.h>
#include <thread/Singleton.h>
#include <thread/Mutex.h>
#include <thread/Lock.h>
#include <thread/Exception.h>
#include "Popuse.h"

class DetailPageThread : public IceUtil::Thread,
                        public MyUtil::Singleton<DetailPageThread>
{
public:
        void start();
        virtual void run();

private:
        int fetch(CPageData* &value, string& filename);
        int parsePage(CPageData* &value, const string& filename);
};


#endif
