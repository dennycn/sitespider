#ifndef _MYTIME_H_
#define _MYTIME_H_
#include <sys/time.h>
#include <time.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;
class Mytime
{
public:
        struct tm* local;
        time_t t;
        Mytime() {

        }
        ~Mytime() {

        }
public:
        void getNowTime(string& nowTime) { //0355 ---3:55
                char buf[100];
                bzero(buf,100);
                t= time(NULL);
                local = localtime(&t);
                sprintf(buf,"%02d%02d",local->tm_hour,local->tm_min);
                nowTime = buf;
        }
        void getNowDate(string& nowDate) { //090330 2009-3-30
                char buf[100];
                bzero(buf,100);
                t = time(NULL);
                local = localtime(&t);
                sprintf(buf,"%02d%02d%02d",local->tm_year-100,local->tm_mon+1,local->tm_mday);
                nowDate = buf;
        }
        void getNowYMDHM(string& nowYMDHM) {
                char buf[100];
                bzero(buf,100);
                t = time(NULL);
                local = localtime(&t);
                sprintf(buf,"%02d%02d%02d%02d%02d",local->tm_year-100,local->tm_mon+1,local->tm_mday,\
                        local->tm_hour,local->tm_min);
                nowYMDHM = buf;
        }
        void getNowSec(int& nowSec) {
                t = time(NULL);
                nowSec = t;
        }
        string getNowSec() {
                t = time(NULL);
                int nowSec = t;;
                char buf[64];
                bzero(buf,64);
                sprintf(buf,"%d",nowSec);
                string ret;
                ret = buf;
                return ret;
        }
        string getInverseSec(int nowSec) {
                string tmpTime;
                char buf[100];
                bzero(buf,100);
                t = nowSec+60;
                local = localtime(&t);
                sprintf(buf,"%02d%02d%02d%02d%02d",local->tm_year-100,local->tm_mon+1,local->tm_mday,\
                        local->tm_hour,local->tm_min);
                tmpTime = buf;
                return tmpTime;
        }
        int timeFormatint(string timeStr ,string flgstr) {
                struct tm t;
                int tmpdate = 0;
                int flag ;
                struct tm *local ;
                time_t t1;
                t1 = time(NULL);
                local = localtime(&t1);
                if ((signed int)flgstr.find("yyyy年mm月dd日") >= 0 ) {
                        flag = (signed int)timeStr.find("年");
                        if (flag > 0) {
                                t.tm_year = atoi(timeStr.c_str()) - 1900;
                                timeStr = timeStr.substr(flag+2, timeStr.size()- flag - 2);
                        } else {
                                t.tm_year = local->tm_year ;
                        }
                        flag = (signed int)timeStr.find("月");
                        if (flag > 0) {
                                t.tm_mon = atoi(timeStr.c_str())-1;
                                timeStr = timeStr.substr(flag+2,timeStr.size()-flag-2);
                        } else {
                                t.tm_mon = local->tm_mon;
                        }
                        flag = (signed int)timeStr.find("日");
                        if (flag > 0) {
                                t.tm_mday = atoi(timeStr.c_str());
                                timeStr = timeStr.substr(flag+2,timeStr.size()-flag-2);
                        } else {
                                t.tm_mday= local->tm_mday;
                        }
                        flag = (signed int)timeStr.find(":");
                        if (flag>0) {
                                t.tm_hour = atoi(timeStr.c_str());
                                timeStr = timeStr.substr(flag+1,timeStr.size()-flag-1);
                        } else {

                                t.tm_hour = local->tm_hour;
                        }
                        if (timeStr[0] >= '0' && timeStr[0] <= '9')
                                t.tm_min = atoi(timeStr.c_str());
                        else {
                                t.tm_min = local->tm_min;
                        }
                        t.tm_sec = 1;
                        t.tm_isdst = 0;
                        tmpdate  = (int)mktime(&t);
                        return tmpdate;
                } else if ( (signed int)flgstr.find("yyyy-mm-dd hh:mm") >= 0) {
                        flag = (signed int)timeStr.find("-");
                        ///cout<<timeStr<<endl;
                        if (flag > 2 && timeStr[0] == '2' && timeStr[1] =='0') {
                                t.tm_year = atoi(timeStr.c_str()) - 1900;
                        } else {
                                t.tm_year = local->tm_year ;
                        }
                        ///cout<<timeStr<<endl;
                        if (flag > 0) {
                                timeStr = timeStr.substr(flag+1 , timeStr.size() - flag - 1);
                                t.tm_mon = atoi(timeStr.c_str()) - 1;
                        } else {

                                t.tm_mon = local->tm_mon;
                        }
                        ////cout<<timeStr<<endl;
                        flag = (signed int)timeStr.find("-");
                        if (flag > 0) {
                                timeStr = timeStr.substr(flag+1 , timeStr.size() - flag - 1);
                                t.tm_mday= atoi(timeStr.c_str());
                        } else {
                                t.tm_mday= local->tm_mday;
                        }
                        ////cout<<timeStr<<endl;
                        if (flag > 0) {
                                timeStr = timeStr.substr(flag+1 , timeStr.size() - flag - 1);
                                t.tm_hour = atoi(timeStr.c_str());
                        } else {
                                t.tm_hour= local->tm_hour;
                        }
                        ////cout<<timeStr<<endl;
                        flag = (signed int)timeStr.find(":");
                        if (flag > 0) {
                                timeStr = timeStr.substr(flag+1 , timeStr.size() - flag - 1);
                                t.tm_min= atoi(timeStr.c_str());
                        } else {
                                t.tm_min= local->tm_min;
                        }
                        ////cout<<timeStr<<endl;
                        t.tm_sec = 1;
                        t.tm_isdst = 0;
                        tmpdate  = (int)mktime(&t);
                        return tmpdate;
                }
                return tmpdate;
        }
        int updateFormatint(string tmpstr2 ,string flgstr) {
                struct tm t;
                int tmpdate;
                int flag ;
                struct tm *local ;
                time_t t1;
                if ((signed int)flgstr.find("google") >= 0) {
                        t1 = time(NULL);
                        local = localtime(&t1);
                        flag = (signed int)tmpstr2.find("月");
                        if (flag > 0) {
                                t.tm_year = local->tm_year;
                                t.tm_mon = atoi(tmpstr2.c_str())-1;
                                tmpstr2 = tmpstr2.substr(flag+2,tmpstr2.size()-flag-2);
                        } else {
                                t.tm_year = local->tm_year;
                                t.tm_mon  = local->tm_mon;
                        }
                        flag = (signed int)tmpstr2.find("日");
                        if (flag > 0) {
                                t.tm_mday = atoi(tmpstr2.c_str())-1;
                                tmpstr2 = tmpstr2.substr(flag+2,tmpstr2.size()-flag-2);
                        } else {
                                t.tm_mday = local->tm_mday;

                        }
                        flag = (signed int)tmpstr2.find("小时");
                        if (flag > 0) {
                                t.tm_hour = local->tm_hour-atoi(tmpstr2.c_str());
                                if (t.tm_hour<0) {
                                        t.tm_hour +=24;
                                }
                                tmpstr2 = tmpstr2.substr(flag+4,tmpstr2.size()-flag-4);
                        } else {
                                t.tm_hour = local->tm_hour;
                        }
                        flag = (signed int)tmpstr2.find("分钟");
                        if (flag>0) {
                                if (flag > 3) {
                                        tmpstr2 = tmpstr2.substr(flag-2,tmpstr2.size()-flag+2);
                                        //	Chisp_ChangeAstr(tmpstr2);
                                }
                                t.tm_min = local->tm_min-atoi(tmpstr2.c_str());
                                if (t.tm_min<0) {
                                        t.tm_min +=60;
                                }
                        } else {
                                t.tm_min = local->tm_min;
                        }
                        t.tm_sec = 1;
                        t.tm_isdst = 0;
                        tmpdate  = mktime(&t);
                        return tmpdate;
                } else if ( (signed int)flgstr.find("kooxoo") >= 0) { //mm-dd ttimeago
                        t1 = time(NULL);
                        local = localtime(&t1);
                        flag = (signed int)tmpstr2.find("-");
                        if (flag > 0) {
                                t.tm_year = local->tm_year;
                                t.tm_mon = atoi(tmpstr2.c_str())-1;
                                tmpstr2 = tmpstr2.substr(flag+1,tmpstr2.size()-flag-1);
                        } else {
                                t.tm_year = local->tm_year;
                                t.tm_mon  = local->tm_mon;
                        }
                        flag = (signed int)tmpstr2.find(" ");
                        if (flag > 0) {
                                t.tm_mday = atoi(tmpstr2.c_str())-1;
                                tmpstr2 = tmpstr2.substr(flag+1,tmpstr2.size()-flag-1);
                        } else {
                                t.tm_mday = local->tm_mday;
                        }
                        flag = (signed int)tmpstr2.find("小时");
                        if (flag > 0) {
                                t.tm_hour = local->tm_hour-atoi(tmpstr2.c_str());

                        }
                        tmpstr2 = tmpstr2.substr(flag+4,tmpstr2.size()-flag-4);
                } else {
                        t.tm_hour = local->tm_hour;
                }
                flag = (signed int)tmpstr2.find("分钟");
                if (flag>0) {
                        if (flag > 3) {
                                tmpstr2 = tmpstr2.substr(flag-2,tmpstr2.size()-flag+2);
                                //	Chisp_ChangeAstr(tmpstr2);
                        }
                        t.tm_min = local->tm_min-atoi(tmpstr2.c_str());
                        if (t.tm_min<0) {
                                t.tm_min +=60;
                        }
                } else {
                        t.tm_min = local->tm_min;
                }
                t.tm_sec = 1;
                t.tm_isdst = 0;
                tmpdate  = mktime(&t);
                return tmpdate;
        }
};

#endif

