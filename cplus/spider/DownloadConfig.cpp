/**
 * @file       DownloadConfig.cpp
 * @brief
 * @author     hanwei
 * @version    1.0.0
 */

/*********************************************************************
 *  hanwei 01/21/10      Initial Version
 *********************************************************************/
#include	"DownloadConfig.h"
#include	<kittylog/KittyLog.h>

DownloadConfig::DownloadConfig()
{
        textTempletNum = 0;
        md5DownTree = new BigSearch<INT64>(1048576, 512, 8);
        pthread_mutex_init(&downLock, NULL);        
//        textQueueNum = 0;
//        sendQueueNum = 0;
//
//        IPchange = false;
//        //md5SendTree = new BigSearch<INT64>(1048576, 512, 8);
//        pthread_mutex_init(&logLock, NULL);
//        pthread_mutex_init(&md5treeLock, NULL);
//
        pthread_mutex_init(&textLock, NULL);
//        pthread_mutex_init(&vectorLock, NULL);
//        pthread_mutex_init(&titleThreadNumLock, NULL);
//        pthread_mutex_init(&textThreadNumLock, NULL);
//        pthread_mutex_init(&sendLock, NULL);
        pthread_mutex_init(&writeLock, NULL);
//        
        configPathInit();
}

DownloadConfig::~DownloadConfig()
{
//        md5SaveTree();
//        if (md5DownTree != NULL)
//                delete md5DownTree;
//        md5DownTree = NULL;
//        if (md5UndownTree != NULL)
//                delete md5UndownTree;
//        md5UndownTree = NULL;
//        recyler();
//        pthread_mutex_destroy(&logLock);
//        pthread_mutex_destroy(&md5treeLock);
//
//        pthread_mutex_destroy(&textLock);
//        pthread_mutex_destroy(&undownLock);
//        pthread_mutex_destroy(&downLock);
//        pthread_mutex_destroy(&sendLock);
//        pthread_mutex_destroy(&vectorLock);
//        pthread_mutex_destroy(&writeLock);
//        pthread_mutex_destroy(&titleThreadNumLock);
//        pthread_mutex_destroy(&textThreadNumLock);
}

void DownloadConfig::recyler()
{
        /*
            Templet_Data* delTemplet = NULL;
            Templet_Data* nextTemplet = NULL;
            CPageData* delPop = NULL;
            while (vectorTem.size() > 0)
            {
                delTemplet = vectorTem.back();
                vectorTem.pop_back();
                while (delTemplet != NULL)
                {
                    nextTemplet  = delTemplet;
                    delTemplet = delTemplet->getNext();
                    delete nextTemplet;
                    nextTemplet = NULL;
                }
            }


            cout<<"Title tem num = "<<titleTemplatePos<<endl;
            cout<<"Vector title temsize = "<<titleTemVec.size()<<endl;
            while (titleTemVec.size() > 0)
            {
                delTemplet = titleTemVec.back();
                while (delTemplet != NULL)
                {
                    nextTemplet  = delTemplet;
                    delTemplet = delTemplet->getNext();
                    delete nextTemplet;
                    nextTemplet = NULL;
                }
            }

            while (textTemVec.size() > 0)
            {
                delTemplet = textTemVec.back();
                textTemVec.pop_back();
                while (delTemplet != NULL)
                {
                    nextTemplet  = delTemplet;
                    delTemplet = delTemplet->getNext();
                    delete nextTemplet;
                    nextTemplet = NULL;
                }
            }

            delPop = NULL;
            // Note:
            while (titleVector.size() > 0)
            {
                delPop = titleVector.back();
                titleVector.pop_back();
                if (delPop != NULL)
                    delPop = NULL;
                delPop = NULL;
            }

            while (vectorVector.size() > 0)
            {
                delPop = vectorVector.back();
                vectorVector.pop_back();
                if (delPop != NULL)
                    delete delPop;
                delPop = NULL;
            }

            while (textQueue.size() != 0)
            {
                delPop = textQueue.front();
                textQueue.pop();
                textQueueNum--;
                if (delPop != NULL)
                {
                    delete delPop;
                    delPop = NULL;
                    //cout<<"Del pop"<<endl;
                }
            }
          */
}

void DownloadConfig::run()
{
}

void DownloadConfig::writeFile (CPageData* &tmpValue)
{
        pthread_mutex_lock(&writeLock);
        //		cout<<"<item_N>"<<endl;
        //		cout<<"<Now><![CDATA["<<tmpValue->now<<"]]></Now>"<<endl;
        //		cout<<"<Title><![CDATA["<<tmpValue->title<<"]]></Title>"<<endl;
        //		cout<<"<TextUrl><![CDATA["<<tmpValue->textUrl<<"]]></TextUrl>"<<endl;
        //		cout<<"<TitleUrl><![CDATA["<<tmpValue->titleUrl<<"]]></TitleUrl>"<<endl;
        //		cout<<"<Date><![CDATA["<<tmpValue->publishTime<<"]]></Date>"<<endl;
        //		cout<<"<Media><![CDATA["<<tmpValue->media<<"]]></Media>"<<endl;
        //		cout<<"<MediaUrl><![CDATA["<<tmpValue->mediaUrl<<"]]></MediaUrl>"<<endl;
        //		cout<<"<Label><![CDATA["<<tmpValue->label<<"]]></Label>"<<endl;
        //		cout<<"<Text><![CDATA["<<tmpValue->text<<"]]></Text>"<<endl;
        //		cout<<"<TextHtml><![CDATA["<<tmpValue->textHtml<<"]]></TextHtml>"<<endl;
        //		cout<<"</item_N>"<<endl;

        dateName = getDateName();
        saveName =  savePath + dateName;
        ofstream outfileSave(saveName.c_str(), ios::out|ios::app);
        outfileSave<<"<item_N>"<<endl;
        outfileSave<<"<Now><![CDATA["<<tmpValue->getNow()<<"]]></Now>"<<endl;
        outfileSave<<"<Title><![CDATA["<<tmpValue->getTitle()<<"]]></Title>"<<endl;
        outfileSave<<"<TextUrl><![CDATA["<<tmpValue->getTextUrl()<<"]]></TextUrl>"<<endl;
        outfileSave<<"<TitleUrl><![CDATA["<<tmpValue->getTitleUrl()<<"]]></TitleUrl>"<<endl;
        outfileSave<<"<Date><![CDATA["<<tmpValue->getPublishTime()<<"]]></Date>"<<endl;
        outfileSave<<"<Media><![CDATA["<<tmpValue->getMedia()<<"]]></Media>"<<endl;
        outfileSave<<"<MediaUrl><![CDATA["<<tmpValue->getMediaUrl()<<"]]></MediaUrl>"<<endl;
        outfileSave<<"<Label><![CDATA["<<tmpValue->getLabel()<<"]]></Label>"<<endl;
        outfileSave<<"<Text><![CDATA["<<tmpValue->getText()<<"]]></Text>"<<endl;
        outfileSave<<"<Website><![CDATA["<<tmpValue->getWebsite()<<"]]></Website>"<<endl;
        outfileSave<<"<TextHtml><![CDATA["<<tmpValue->getTextHtml()<<"]]></TextHtml>"<<endl;
        outfileSave<<"</item_N>"<<endl;
        outfileSave.close();
        pthread_mutex_unlock(&writeLock);

}		// -----  end of function writeFile  -----
// ===  FUNCTION  ======================================================================
//  Name:  configInit
//  Description:  it will read the txt/Config.txt and load some download data
// =====================================================================================
int DownloadConfig::configInit()
{
        ifstream infileConfig(configName.c_str(),ios::in);
        assert(infileConfig != NULL);
        if ( !infileConfig ){
                KittyLog::debug( "fopen error: %s", configName.c_str());
                return -1;
        }

        // infileConfig:
        string str;
        while (getline(infileConfig, str)) {
                if ('#' == str[0])
                        continue;
                if (str.compare(0, 11, "Stopsignal=") == 0) {
                        Stop_signal = str[11];
                        continue;
                }
                if (str.compare(0, 14, "DetailPageThreadNum=") == 0) {
                        //str = str.substr(14, str.size() - 14);
                        //textThreadNum = atoi(str.c_str());
                        continue;
                }
                if (str.compare(0, 15, "ListPageThreadNum=") == 0) {
                        //str = str.substr(15, str.size() -15);
                        //titleThreadNum = atoi(str.c_str());
                        continue;
                }
                if (str.compare(0, 13, "TempletTitle=") == 0) {

                        continue;
                }
                if (str.compare(0, 12, "TempletText=") == 0) {
                        str = str.substr(12, str.size()-12);
                        char tmpname[1024];
                        bzero(tmpname, 1024);
                        str = txtPath + str;
                        StringMore::removeLastStr(str);
                        strcpy(tmpname, str.c_str());
                        struct stat statbuf;
                        int tmpflg;
                        tmpflg = stat(tmpname, &statbuf);
                        if (-1 == tmpflg)
                                continue;
                        ifstream infile(tmpname, ios::in);
                        if (infile == NULL) {
                                KittyLog::error("TempletText Open Error");
                                if (0 == tmpflg)
                                        cout<<"File Log Open Error"<<endl;
                                continue;
                        }
                        tmpflg = 0;//used for the NUMB flag
                        int tmpNum = -1;
                        Templet_Data* tmpTemplet = NULL;
                        Templet_Data* tmpTempletNext = NULL;
                        while (getline(infile, str)) {
                                StringMore::removeLastStr(str);
                                if ('#' == str[0])
                                        continue;
                                if (str.compare(0, 7, "!$NUMB=") == 0) {
                                        str = str.substr(7, str.size()-7);
                                        tmpNum = atoi(str.c_str());
                                        ///assert(textTempletNum == tmpNum);
                                        textTempletNum++;
                                        tmpflg = 1;
                                        continue;
                                }
                                if (str.compare(0, 7, "/$NUME=") == 0) {
                                        continue;
                                }
                                if ((signed int)str.find("_b") > 0) {
                                        if (tmpflg == 1) {
                                                tmpflg = 0;
                                                tmpTemplet = NULL;
                                                tmpTemplet = new Templet_Data;
                                                textTemVec.push_back(tmpTemplet);
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
                        KittyLog::info("detailpage TempletNum = %d", textTempletNum);
                        continue;
                }
        }
        infileConfig.close();
        return 0;
}
// ===  FUNCTION  ======================================================================
//  Name:  configPathInit
//  Description:  some Path init will create in this function
// =====================================================================================
void DownloadConfig::configPathInit()
{
        char tmpchar[1024];
        bzero(tmpchar,sizeof(tmpchar));
        ///assert(NULL != getcwd(tmpchar,1024));
        filePath = tmpchar;
        ///assert(StringMore::removeLastStr(filePath) > 5);
        //if (filePath[filePath.length() -1] != '/')
          //      filePath += "/";
        //end of the filepath get
        txtPath = filePath + "conf/";
        md5Path = filePath + "save/";
        logPath = filePath + "mirrors/";
        savePath= filePath + "save/";
        mkdir(md5Path.c_str(),  0777);
        mkdir(logPath.c_str(),  0777);
        mkdir(savePath.c_str(), 0777);
        //end of the mkdir

        configName = txtPath + "system.ini";
//        configChangeName = txtPath + "ConfigChange.txt";
//        md5DownName = md5Path + "md5Downdata.dat";
//        md5UnDownName = md5Path + "md5UnDownData.dat";
//
//        dateName = getDateName();
        logName = logPath + dateName;
        saveName = savePath + dateName;

}
// ===  FUNCTION  ======================================================================
//  Name:  md5LoadTree
//  Description:  it will load the md5Downdata.dat will be laod
// =====================================================================================
//int DownloadConfig::md5LoadTree()
//{
//        if (md5DownName.length() > 10)
//                md5DownTree->LoadTree( md5DownName.c_str(), NULL);
//        if (md5DownName.length() > 10)
//                md5UndownTree->LoadTree( md5DownName.c_str(), NULL);
//        return 1;
//}
//// ===  FUNCTION  ======================================================================
//  Name:  md5SaveTree
//  Description:  md5 down tree download save
// =====================================================================================
//int DownloadConfig::md5SaveTree()//it need to use lock error:-1 success:0
//{
//        int tmpflg = 0;
//        pthread_mutex_lock(&md5treeLock);
//        if (md5DownName.length() > 10)
//                tmpflg = md5DownTree->SaveTree(md5DownName.c_str());
//        pthread_mutex_unlock(&md5treeLock);
//        return tmpflg;
//}
//
// ===  FUNCTION  ======================================================================
//  Name:  makeFileName
//  Description:  make only name from the value
// =====================================================================================
// used by list and detailpage thread
string DownloadConfig::makeFileName(const string value)
{
        INT64 md5Value = 0;
        string ret;
        char tmpLogName[100];
        memset(tmpLogName, 0, sizeof(tmpLogName));
        MD5String(value.c_str(), (unsigned char*)& md5Value, 8);
        sprintf(tmpLogName,"mirrors/%x.dat",(unsigned)md5Value);
        ret = tmpLogName;
        return ret;
}
// ===  FUNCTION  ======================================================================
//  Name:  getDateName
//  Description:  get the date of today
// =====================================================================================
string  DownloadConfig::getDateName()//need to acculmulate
{
        string value;
        time_t lt;
        struct tm *Nt;
        lt = time(NULL);
        Nt = localtime(&lt);
        char tmpbuf[64];
        bzero(tmpbuf, sizeof(tmpbuf));
        sprintf(tmpbuf, "%02d%02d%02d%02d",Nt->tm_year-100,Nt->tm_mon+1, Nt->tm_mday, Nt->tm_hour);
        value = tmpbuf;
        return value;
}

// ===  FUNCTION  ======================================================================
//  Name:  getStopSignal
//  Description:  get the Stop_signal of the global flag
// =====================================================================================
char DownloadConfig::getStopSignal()
{
        return Stop_signal;
}


// ===  FUNCTION  ======================================================================
//  Name:  delListPageThreadNum
//  Description:  del title thread
// =====================================================================================
//void DownloadConfig::delListPageThreadNum ()
//{
//        pthread_mutex_lock(&titleThreadNumLock);
//        titleThreadNum--;
//        pthread_mutex_unlock(&titleThreadNumLock);
//}		// -----  end of function delListPageThreadNum  -----
//
//
//int DownloadConfig::getDetailPageThreadNum ()
//{
//        return textThreadNum;
//}		// -----  end of function getDetailPageThreadNum  -----
//// ===  FUNCTION  ======================================================================
//  Name:  delDetailPageThreadNum
//  Description:  det the text thread num
// =====================================================================================
//void DownloadConfig::delDetailPageThreadNum ()
//{
//        pthread_mutex_lock(&textThreadNumLock);
//        textThreadNum--;
//        pthread_mutex_unlock(&textThreadNumLock);
//}		// -----  end of function delDetailPageThreadNum  -----
//// ===  FUNCTION  ======================================================================
		// -----  end of function checkDownMsg  -----

// ===  FUNCTION  ======================================================================
//  Name:  setTextTask
//  Description:  push the textTask in the textQueue;
// =====================================================================================
void DownloadConfig::setTextTask (CPageData* &tmpValue)
{
        pthread_mutex_lock(&textLock);
        textQueue.push(tmpValue);
        textQueueNum++;
        pthread_mutex_unlock(&textLock);
}
//// ===  FUNCTION  ======================================================================
//  Name:  getTextTask
//  Description:  get the text Task;
//  return     :   0 true -1 false;  and should delete the task outside 1 sleep;
// =====================================================================================
int DownloadConfig::getTextTask (CPageData* &tmpValue)
{
        if (textQueue.size() == 0) {
                return 1;
        }
        pthread_mutex_lock(&textLock);
        tmpValue = textQueue.front();
        textQueue.pop();
        textQueueNum = textQueue.size();
        pthread_mutex_unlock(&textLock);
        if (tmpValue != NULL)
                return 0;
        else
                return -1;
}


// ===  FUNCTION  ======================================================================
//  Name:  getTextTemplet
//  Description:  get the text templet
// =====================================================================================
Templet_Data* DownloadConfig::getTextTemplet (int tmpi)
{
        if (tmpi >= textTempletNum)
                return NULL;
        else
                return textTemVec[tmpi];
}		// -----  end of function getTextTemplet  -----


// ===  FUNCTION  ======================================================================
//  Name:  setSendTask
//  Description:  push the send task in the sendQueue
// =====================================================================================
/*
void DownloadConfig::setSendTask (CPageData* &tmpValue)
{
    pthread_mutex_lock(&sendLock);
    sendQueue.push(tmpValue);
    sendQueueNum++;
    pthread_mutex_unlock(&sendLock);
}		// -----  end of function setSendTask  -----
// ===  FUNCTION  ======================================================================
//  Name:  getSendTask
//  Description:  pop the send task in the sendQueue
//  return :      -1 no send task 0 exist send task
// =====================================================================================
int DownloadConfig::getSendTask (CPageData* &tmpValue)
{
    if (sendQueue.size() == 0)
        return -1;
    pthread_mutex_lock(&sendLock);
    tmpValue = sendQueue.front();
    sendQueue.pop();
    sendQueueNum = sendQueue.size();
    pthread_mutex_unlock(&sendLock);
    return 0;
}		// -----  end of function getSendTask  -----
// ===  FUNCTION  ======================================================================
//  Name:  getSendQueueNum
//  Description:  get send Queue num
// =====================================================================================
int DownloadConfig::getSendQueueNum ()
{
    return sendQueue.size();
}		// -----  end of function getSendQueueNum  -----
*/

// ===  FUNCTION  ======================================================================
//  Name:  checkDownMsg
//  Description:  check the text whether is usefurl
//       return:  0 exist !0 not exist
// =====================================================================================
int DownloadConfig::checkDownMsg (const string& value)
{
        INT64 tmpMd5 = 0;
        void* tmpValue = NULL;
        void* tmpValueOld = NULL;
        int tmpflg = 0;
        int tmpNumIn = 1;
        MD5String(value.c_str(), (unsigned char*)&tmpMd5, 8);
        pthread_mutex_lock(&downLock);
        tmpflg = md5DownTree->Search(tmpMd5,tmpValue);
        if ( 0 != tmpflg && 0 != tmpMd5) {
                md5DownTree->add(tmpMd5,&tmpNumIn,tmpValueOld);
        }
        pthread_mutex_unlock(&downLock);
        return tmpflg;

}

