//=====================================================================================
// Filename:  main.cpp
// Description:  the main of the ihandy_ycnews1.0
// Author:
//=====================================================================================

#include <kittylog/KittyLog.h>
#include <gflags/gflags.h>

#include <thread/Thread.h>
#include <thread/Singleton.h>
#include <thread/Exception.h>
#include <thread/TaskManager.h>

#include "defines.h"
#include "ListPageMonitorThread.h"
#include "DetailPageThread.h"
#include "DownloadConfig.h"
//#include "Charecter.h"
//#include "SendThread.h"

// the two function have no use now
void CtrlStop(void* par);
int getClassTag(const char* file);

using namespace std;
using namespace MyUtil;
using std::map;
map<string, int> fenleiMap;

DEFINE_bool( daemon, false, "daemon the program" );

int main ( int argc, char *argv[] )
{
        //parse commandline
        std::string helpMessage = "Usage:\t";
        helpMessage += argv[0];
        helpMessage += "\n\t--daemon=false|true";
        helpMessage += "\n\t\t: whether daemon the programe,default to false.";
        helpMessage += "\n\t--help";
        helpMessage += "\n\t\t: show help information.";
        google::SetUsageMessage( helpMessage );
        google::ParseCommandLineFlags( &argc, &argv, true );

        //init log
        KittyLog::kitty_log_init( "log4cplus.properties" );
        KittyLog::info("spider start...");
        if ( FLAGS_daemon ) {
                KittyLog::info("will daemon the program...");
                daemon(1, 0);
        }

        // get classtag map
        getClassTag("conf/fenleiBiao.txt");

        // use threadpool
        int iMinWorkThreadNum = 1;
        int iMaxWorkThreadNum = 1;
        TaskManager::instance().initialize( iMinWorkThreadNum, iMaxWorkThreadNum );

        KittyLog::info("DownloadConfig init %d", DownloadConfig::instance().configInit());
        ListPageMonitorThread::instance().start();
        DetailPageThread::instance().start();
#ifdef SENDFLG
        //SendThread::instance().start();
#endif
        
        while (1) {
                //if (DownloadConfig::instance().getDetailPageThreadNum() == 0 && DownloadConfig::instance().getListPageThreadNum() == 0) {
                // break;
                //}
                sleep(30);
        }
        KittyLog::debug("O text thread in the task");

        cout<<"Spider end."<<endl;
        return EXIT_SUCCESS;
} // ----------  end of function main  ----------


// ===  FUNCTION  ======================================================================
//  Name:  CtrlStop
//  Description:  It's used for control the global program, haven't use it now.
//  1 md5tree update
//  2 templet or ip update
//  3 Initnew produce
//  4 write file
//  5 write log
// =====================================================================================
void CtrlStop (void* tmpConfig)
{
//        string configName;
//        while (DownloadConfig::instance().getDetailPageThreadNum() > 0) {
//                sleep(120);
//                DownloadConfig::instance().md5SaveTree();
//                //DownloadConfig::instance().configChange();
//        }
//
        return ;
}		// -----  end of function CtrlStop  -----

// classtag use in generate map for sendThread
int getClassTag(const char* file)
{
        ifstream infile(file, ios::in);
        string str;
        int num;
        while (getline(infile, str)) {
                num  = (signed int)str.find("|");
                string tmpstr;
                tmpstr = str.substr(0, num);
                str = str.substr(num+1, str.size() - num -1);
                num = atoi(str.c_str());
                fenleiMap[tmpstr] = num;
        }
        infile.close();
        return 0;
}

