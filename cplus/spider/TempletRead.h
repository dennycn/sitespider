#ifndef _TEMPLETREAD_H_
#define _TEMPLETREAD_H_

#include <string>
#include <queue>
#include "Popuse.h"
#include "Templet.h"
#include "StringMore.h"
#include "DownloadConfig.h"

using namespace std;
using namespace StringMore;
class DownloadConfig;

class TempletRead
{
public:
        queue<CPageData*> taskStore;
        int taskNum;
public:
        int ParseText(CPageData* &taskValue, string& htmlbuf, char*& strbuf);
        int ParseTitle(CPageData* &taskValue, string& htmlValue);
        int getFormatUrl(string& fixedStr, string& fatherUrl);
        TempletRead();
        ~TempletRead();
};
#endif

