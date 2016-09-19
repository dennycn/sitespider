/**
 * @file       Popuse.cpp
 * @brief
 * @author    Hanwei
 * @version    1.0.0
 */

#include "Popuse.h"
#include <kittylog/KittyLog.h>

void CPageData::setText(string tmpvalue)
{
        if (!tmpvalue.empty() )
                text = tmpvalue;
}
string CPageData::getText()
{
        return text;
}

string CPageData::getTitle()
{
        return title;
}
void CPageData::setTextHtml(string tmpvalue)
{
        if (!tmpvalue.empty())
                textHtml = tmpvalue;
}
void  CPageData::setDate(string tmpvalue, string tmpstrFlg)
{
        publishTime = tmpvalue;
        StringMore::removeSpecial(publishTime);
        int flg;
        flg = (signed int)publishTime.find("2009");
        if (flg >= 0) {
                publishTime = publishTime.substr(flg, publishTime.size()-flg);
        }
        flg = (signed int)publishTime.find("2010");
        if (flg >= 0) {
                publishTime = publishTime.substr(flg, publishTime.size()-flg);
        }
        if (publishTime.size() >= 25) {
                publishTime.substr(0, 23);
        }
        flg = (signed int)publishTime.find(":");
        if (flg > 0) {
                flg += 3;
                publishTime = publishTime.substr(0, flg);
        }
        Mytime  mytime;
        now = 	mytime.timeFormatint(publishTime, tmpstrFlg);
}		// -----  end of function setDate  -----
string  CPageData::getDate ()
{
        return publishTime;
}		// -----  end of function getDate  -----
void CPageData::setMedia (string tmpvalue)
{
        int tmpi, tmpj;
        string tmpstr;
        if ((tmpi = (signed int)tmpvalue.find("http://")) >= 0) {
                if ((tmpj  = (signed int)tmpvalue.find(">")) > 0 )
                        tmpstr =tmpvalue.substr(tmpi, tmpj);
                setMediaUrl(tmpstr);
                //tmpvalue = tmpvalue.substr(tmpj, tmpvalue.size() - tmpj);
        }
        StringMore::removeHtml(tmpvalue);
        media = tmpvalue;
        StringMore::removeMiddleStr(media);
        //	cout<<"Media=Chinese"<<media<<endl;
}		// -----  end of function setMedia  -----
string CPageData::getMedia ()
{
        return media;
}		// -----  end of function getMedia  -----

void CPageData::setMediaUrl(string tmpvalue)
{
        if (!tmpvalue.empty()) {
                mediaUrl = tmpvalue;
                int tmpi;
                if ((tmpi = (signed int)mediaUrl.find("http://")) >= 0) {
                        mediaUrl = mediaUrl.substr(tmpi, mediaUrl.size() - tmpi);
                        if ((tmpi = (signed int)mediaUrl.find(" ")) > 0) {
                                mediaUrl = mediaUrl.substr(0, tmpi);
                        } else if ((tmpi = (signed int)mediaUrl.find(">")) > 0) {
                                mediaUrl = mediaUrl.substr(0, tmpi);
                        } else if ((tmpi = (signed int)mediaUrl.find("'")) > 0) {
                                mediaUrl = mediaUrl.substr(0, tmpi);
                        }
                        if ((tmpi = (signed int)mediaUrl.find("\""))> 0)
                                mediaUrl = mediaUrl.substr(0, tmpi);
                }
        }
}
string CPageData::getMediaUrl ()
{
        return mediaUrl;
}		// -----  end of function getMediaUrl  -----
void CPageData::setTitle (string tmpvalue)
{
        if (!tmpvalue.empty())
                title = tmpvalue;
}		// -----  end of function setTitle  -----
void CPageData::setTextUrl (string tmpvalue)
{
        if (!tmpvalue.empty())
                strncpy(textUrl, tmpvalue.c_str(), 254);
}		// -----  end of function setTextUrl  -----
char*  CPageData::getTextUrl()
{
        return textUrl;
}
int CPageData::getNow()
{
        return now;
}
string CPageData::getPublishTime()
{
        return publishTime;
}
string CPageData::getLabel()
{
        return label;
}
string CPageData::getWebsite()
{
        return website;
}
char*  CPageData::getTitleUrl()
{
        return titleUrl;
}
string CPageData::getTextHtml()
{
        return textHtml;
}
int CPageData::getTranscode()
{
        return transcode;
}
int CPageData::getDownloadId()
{
        return downloadId;
}
char CPageData::getDownloadFlg()
{
        return downloadFlg;
}
int CPageData::makeNewsson(CPageData* taskData)
{
        taskData->downloadId = this->downloadId;
        taskData->modifyType = this->modifyType;
        strcpy(taskData->listUrl, this->listUrl);
        strcpy(taskData->titleUrl, this->titleUrl);
        taskData->listTemNum = this->listTemNum;
        taskData->titlePos = this->titlePos;
        taskData->textTemNum = this->textTemNum;
        taskData->post = this->post;
        taskData->passwd = this->passwd;
        taskData->label = this->label;
        taskData->website = this->website;
        taskData->property = 'T';
        taskData->downloadFlg = this->downloadFlg;
        taskData->writeFileFlg = this->writeFileFlg;
        taskData->transcode = this->transcode;
        taskData->now = this->now;
        taskData->publishTime = this->publishTime;
        taskData->title = this->title;
        taskData->text = this->text;
        taskData->media = this->media;
        taskData->textHtml = this->textHtml;
        taskData->mediaUrl = this->mediaUrl;
        return 0;
}
int CPageData::getTitleTemNum()
{
        return titlePos;
}
int CPageData::getTextTemNum()
{
        return textTemNum;
}
void CPageData::setTitleUrl(string tmpvalue)
{
        if (!tmpvalue.empty())
                strncpy(titleUrl,  tmpvalue.c_str(), 254);
}
void CPageData::setListUrl(string tmpvalue)
{
        if (!tmpvalue.empty())
                strncpy(listUrl, tmpvalue.c_str(), 254);
}
void CPageData::setModifyType(string tmpvalue)
{
        if (!tmpvalue.empty())
                modifyType = tmpvalue;
}
void CPageData::setDownloadId(int tmpvalue)
{
        downloadId = tmpvalue;
}
void CPageData::setProperty(char tmpvalue)
{
        property = tmpvalue;
}
void CPageData::setListTemNum(int tmpvalue)
{
        listTemNum = tmpvalue;
}
void CPageData::setTitleTemNum(int tmpvalue)
{
        titlePos = tmpvalue;
}
void CPageData::setTextTemNum(int tmpvalue)
{
        textTemNum = tmpvalue;
}
void CPageData::setPost(string tmpvalue)
{
        post = tmpvalue;
}
void CPageData::setPasswd(string tmpvalue)
{
        passwd = tmpvalue;
}
void CPageData::setLabel(string tmpvalue)
{
        label = tmpvalue;
}
void CPageData::setWebsite(string tmpvalue)
{
        website = tmpvalue;
}
void CPageData::setDownloadFlg(char tmpvalue)
{
        downloadFlg = tmpvalue;
}
void CPageData::setWriteFileFlg(int tmpvalue)
{
        writeFileFlg = tmpvalue;
}
void CPageData::setTranscode(int tmpvalue)
{
        transcode = tmpvalue;
}
void CPageData::setImgUrl(string tmpvalue)
{
        imgUrl = tmpvalue;
}
