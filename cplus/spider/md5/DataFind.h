#ifndef __DATA__FIND__H
#define __DATA__FIND__H

#include "MemoryManage.h"
/************************************************************************/
/* 该类主要用于数值的少量数据的增加和删除，数据量以不超过100万为宜
************************************************************************/
template <class T1>
class FewSearch
{
public:
    /************************************************************************/
    /* lDataSize: 容量不大于lDataSize的200倍  lDataSize的算法为
    * 数据个数*(键值大小+4)+所有内容的大小  DataFieldLen = -1 非定长数据,否则为
    * 数据域长度
    ************************************************************************/
    FewSearch(long lDataSize,long lHashSize,long DataFieldLen = -1)
    {
        lHSize = lHashSize;
        Hash = new char *[lHSize];
        memset(Hash,0,lHSize * 4);
        pMemory = NULL;
        pBlockMemory = NULL;
        if(DataFieldLen == -1)
            pMemory = new MemoryManage(200,lDataSize/50 + 1);
        else
            pBlockMemory = new BlockMemoryManage(200,lDataSize/50 + 1,DataFieldLen+sizeof(T1)+4);
        lDataNum = 0;
        m_lDataFieldLen = DataFieldLen;
        m_pTmp = NULL;
        if(DataFieldLen >= 0)
            m_pTmp = new char[DataFieldLen+sizeof(T1)];
    };
    ~FewSearch()
    {
        delete Hash;
        if(pMemory)
            delete pMemory;
        if(pBlockMemory)
            delete pBlockMemory;
        if(m_pTmp)
            delete m_pTmp;
    };
    /************************************************************************/
    /* 回到数据头，取出数据前要调用这个函数
    ************************************************************************/
    long GoDataHead()
    {
        lOutIndex = 0;
        pOut = Hash[0];
        return 0;
    };
    /************************************************************************/
    /* 取出一条数据
    ************************************************************************/
    long GetData(T1 &key,void *&data)
    {
        if(pOut)
        {
            key = *(T1 *)(pOut+4);
            data = (void *)(pOut+4+sizeof(T1));
            pOut = *(char **)pOut;
            return 0;
        }
        for(lOutIndex++; lOutIndex<lHSize; lOutIndex++)
        {
            pOut = Hash[lOutIndex];
            if(pOut == NULL)
                continue;
            key = *(T1 *)(pOut+4);
            data = (void *)(pOut+4+sizeof(T1));
            pOut = *(char **)pOut;
            return 0;
        }
        return -1;
    };
    /************************************************************************/
    /* 删除一个键值
    ************************************************************************/
    long DeleteKey(T1 key)
    {
        if(m_lDataFieldLen == -1)
            return -1;
        long l = (long)(key%lHSize);
        if(l < 0) l = l*-1;
        if(Hash[l] == NULL)
            return -1;
        char *p = Hash[l];
        char *p1 = NULL;
        while(p)
        {
            if(*(T1 *)(p+4) == key)
            {
                if(p1 == NULL)
                    Hash[l] = *(char **)p;
                else
                    *(char **)p1 = *(char **)p;
                pBlockMemory->Free(p);
                return 0;
            }
            p1 = p;
            p = *(char **)p;
        }
        return -1;
    };
    /************************************************************************/
    /* 功能：用于导出定长数据
    * 输入参数：要导出的数据buf,lGetNum:导出个数,lFlag:排序方式,-1降序，1升序,0不排
    * 返回值：成功返回0，否则返回-1
    ************************************************************************/
    long GetFixLenData(void *pOutBuf,long lGetNum,long lFlag=0)
    {
        if(m_lDataFieldLen < 0)
            return -1;
        long k = 0;
        char *p;
        char *pOut = (char*)pOutBuf;
        for(long i = 0 ; i < lHSize && k < lGetNum; i++)
        {
            if(i == 52)
                i = i;
            if(Hash[i] == NULL) continue;
            p = Hash[i];
            while(p)
            {
                memcpy(pOut,p+4,m_lDataFieldLen + sizeof(T1));
                pOut += m_lDataFieldLen + sizeof(T1);
                k++;
                if(k>=lGetNum)
                    break;
                p = *(char **)p;
            }
        }
        if(lFlag > 0)
            SortDataAsc((char*)pOutBuf,m_lDataFieldLen+sizeof(T1),0,k-1);
        else if(lFlag < 0)
            SortDataDesc((char*)pOutBuf,m_lDataFieldLen+sizeof(T1),0,k-1);
        return k;
    }
    /************************************************************************/
    /* 函数功能：返回当前类中存放的数据量
    ************************************************************************/
    long GetDataNum() {
        return lDataNum;
    };
    /************************************************************************/
    /* 函数功能：向系统中增加一条数据，用于数据域的长度为定值时
    * 参数：key 用于查找的键值 data 增加的数据域地址 old如果该键值已经存在，存放原来的数据域地址
    * 返回值：0 成功插入 -1键值已经存在  -2出错，可能是不能分配内存
    ************************************************************************/
    long add(T1 key,void *data,void *&old)
    {
        long l = (long)(key%lHSize);
        if(l < 0) l*=-1;
        if(Hash[l] == NULL)
        {
            if(m_lDataFieldLen == -1)
                Hash[l] = (char*)pMemory->Alloc(sizeof(T1)+m_lDataFieldLen+4);
            else
                Hash[l] = (char*)pBlockMemory->Alloc();
            if(Hash[l] == NULL)
                return -2;
            *(long *)(Hash[l]) = 0;
            *(T1 *)(Hash[l] + 4) = key;
            if(m_lDataFieldLen > 0)
                memcpy(Hash[l] + 4 + sizeof(T1),data,m_lDataFieldLen);
            lDataNum++;
            return 0;
        }
        char *p = Hash[l];
        while(p)
        {
            if(*(T1 *)(p+4) == key)
            {
                old = (void*)(p+4+sizeof(T1));
                return -1;
            }
            p = *(char **)p;
        }
        if(m_lDataFieldLen == -1)
            p = (char*)pMemory->Alloc(sizeof(T1)+m_lDataFieldLen+4);
        else
            p = (char*)pBlockMemory->Alloc();
        if(p == NULL)
            return -2;
        *(long *)p = (long)(Hash[l]);
        *(T1 *)(p + 4) = key;
        if(m_lDataFieldLen>0)
            memcpy(p + 4 + sizeof(T1),data,m_lDataFieldLen);
        Hash[l] = p;
        lDataNum++;
        return 0;
    }
    /************************************************************************/
    /* 函数功能：向系统中增加一条数据，用于数据域的长度为定值时
    * 参数：key 用于查找的键值 data 增加的数据域地址 old如果该键值已经存在，存放原来的数据域地址
    * 返回值：0 成功插入 -1键值已经存在  -2出错，可能是不能分配内存 -3 系统被定义为定值，但插入的数据长度与定义的不符
    ************************************************************************/
    long add(T1 key,void *data,void *&old,long lBufSize)
    {
        if(m_lDataFieldLen >= 0)
        {
            if(lBufSize != m_lDataFieldLen)
                return -3;
        }
        long l = (long)(key%lHSize);
        if(l < 0) l*=-1;
        if(Hash[l] == NULL)
        {
            if(m_lDataFieldLen == -1)
                Hash[l] = (char*)pMemory->Alloc(sizeof(T1)+lBufSize+4);
            else
                Hash[l] = (char*)pBlockMemory->Alloc();
            if(Hash[l] == NULL)
                return -2;
            *(long *)(Hash[l]) = 0;
            *(T1 *)(Hash[l] + 4) = key;
            if(lBufSize > 0)
                memcpy(Hash[l] + 4 + sizeof(T1),data,lBufSize);
            lDataNum++;
            return 0;
        }
        char *p = Hash[l];
        while(p)
        {
            if(*(T1 *)(p+4) == key)
            {
                old = (void*)(p+4+sizeof(T1));
                return -1;
            }
            p = *(char **)p;
        }
        if(m_lDataFieldLen == -1)
            p = (char*)pMemory->Alloc(sizeof(T1)+lBufSize+4);
        else
            p = (char*)pBlockMemory->Alloc();
        if(p == NULL)
            return -2;
        *(long *)p = (long)(Hash[l]);
        *(T1 *)(p + 4) = key;
        if(lBufSize>0)
            memcpy(p + 4 + sizeof(T1),data,lBufSize);
        Hash[l] = p;
        lDataNum++;
        return 0;
    };
    /************************************************************************/
    /* 函数功能：查找
    * 输入参数：key 要查找的键值 data 返回的数据域指针
    * 返回值：0成功，-1 失败
    ************************************************************************/
    long Search(T1 key,void *&data)
    {
        long l = (long)(key%lHSize);
        if(l < 0) l = l*-1;
        if(Hash[l] == NULL)
            return -1;
        char *p = Hash[l];
        while(p)
        {
            if(*(T1 *)(p+4) == key)
            {
                data = p + 4 + sizeof(T1);
                return 0;
            }
            p = *(char **)p;
        }
        return -1;
    };
    /************************************************************************/
    /* 系统初始化，数据被清空
    ************************************************************************/
    void clear()
    {
        memset(Hash,0,lHSize * 4);
        lDataNum = 0;
        if(pMemory)
            pMemory->clear();
        if(pBlockMemory)
            pBlockMemory->clear();
    };
private:
    long SortDataAsc(char *pSort,long lStep,long begin,long end)
    {
        if(begin >= end)
            return 0;
        if(end == begin + 1)
        {
            if(*(T1 *)(pSort + begin * lStep) > *(T1 *)(pSort + end * lStep))
            {
                memcpy(m_pTmp,pSort + begin * lStep,lStep);
                memcpy(pSort + begin * lStep,pSort + end * lStep,lStep);
                memcpy(pSort + end * lStep,m_pTmp,lStep);
            }
            return 0;
        }

        long mid = (begin + end)>>1;
        T1 tMidValue =*(T1 *)(pSort + mid * lStep);
        long m = begin;
        long n = end;

        while(begin < end)
        {
            while(begin < end && *(T1 *)(pSort + begin * lStep) < tMidValue)
                begin ++;
            while(begin < end && *(T1 *)(pSort + end * lStep) > tMidValue)
                end --;

            if(begin < end)
            {
                memcpy(m_pTmp,pSort + begin * lStep,lStep);
                memcpy(pSort + begin * lStep,pSort + end * lStep,lStep);
                memcpy(pSort + end * lStep,m_pTmp,lStep);
                begin++;
                if(begin < end)
                    end--;
            }
        }

        if(*(T1 *)(pSort + begin * lStep) < tMidValue)
            begin++;
        if(m < begin)
            SortDataAsc(pSort,lStep,m, begin-1);
        if(end < n)
            SortDataAsc(pSort,lStep,end, n);
        return 0;
    };
    long SortDataDesc(char *pSort,long lStep,long begin,long end)
    {
        if(begin >= end)
            return 0;
        if(end == begin + 1)
        {
            if(*(T1 *)(pSort + begin * lStep) < *(T1 *)(pSort + end * lStep))
            {
                memcpy(m_pTmp,pSort + begin * lStep,lStep);
                memcpy(pSort + begin * lStep,pSort + end * lStep,lStep);
                memcpy(pSort + end * lStep,m_pTmp,lStep);
            }
            return 0;
        }

        long mid = (begin + end)>>1;
        T1 tMidValue =*(T1 *)(pSort + mid * lStep);
        long m = begin;
        long n = end;

        while(begin < end)
        {
            while(begin < end && *(T1 *)(pSort + begin * lStep) > tMidValue)
                begin ++;
            while(begin < end && *(T1 *)(pSort + end * lStep) < tMidValue)
                end --;

            if(begin < end)
            {
                memcpy(m_pTmp,pSort + begin * lStep,lStep);
                memcpy(pSort + begin * lStep,pSort + end * lStep,lStep);
                memcpy(pSort + end * lStep,m_pTmp,lStep);
                begin++;
                if(begin < end)
                    end--;
            }
        }

        if(*(T1 *)(pSort + begin * lStep) > tMidValue)
            begin++;
        if(m < begin)
            SortDataDesc(pSort,lStep,m, begin-1);
        if(end < n)
            SortDataDesc(pSort,lStep,end, n);
        return 0;
    };
private:
    char **Hash;
    long lHSize;
    MemoryManage *pMemory;
    BlockMemoryManage *pBlockMemory;
    long lDataNum;
    long m_lDataFieldLen;
    long lOutIndex;
    char *pOut;
    char *m_pTmp;

};
/************************************************************************/
/* 该类主要用于数值的大量数据的增加和删除，数据量超过100万为宜，但只适用于数据域为定长时
 系统占用内存的大小计算方法基本为（sizeof(T1)+数据域长度）* 数据量 ，实际占用比该值稍大
************************************************************************/
template <class T1>
class BigSearch
{
public:
    /************************************************************************/
    /* 参数：lBlockSize:每一块内存中存放的数据量
    *       lMaxBlockNum:可以开辟的最大内存块数
    *       lContSize：数据域的大小
    * 说明：lBlockSize * (lMaxBlockNum) 为系统的最大容量
    *       lBlockSize越大，额外内存越多，但是速度越快，但当值达到一定时，速度影响变小，建议使用1<<20
    ************************************************************************/
    BigSearch(long lBlockSize,long lMaxBlockNum,long lContSize)
    {
        lMaxBlockNum--;
        m_lBlockSize = lBlockSize;
        m_lMaxBlockNum = lMaxBlockNum;
        m_lContSize = lContSize;
        m_lOneBlock = m_lContSize + sizeof(T1);
        m_fSearch = new FewSearch<T1>(lBlockSize,lBlockSize+1,lContSize);
        m_pBlockIndex = new char*[lMaxBlockNum+1];
        memset(m_pBlockIndex,0,sizeof(char*) * (lMaxBlockNum+1));
        m_pBlockIndex[lMaxBlockNum] = new char [m_lOneBlock * lBlockSize];
        m_lNowIndex = 0;
        m_lNowData = 0;
        m_fDelSearch = NULL;
    };
    ~BigSearch()
    {
        delete m_fSearch;
        if(m_fDelSearch)
            delete m_fDelSearch;
        for(long i=0; i<=m_lMaxBlockNum; i++)
        {
            if(m_pBlockIndex[i]) delete m_pBlockIndex[i];
        }
        delete m_pBlockIndex;
    };
    /************************************************************************/
    /*准备得到数据，在顺序输出之前一定要调用该函数,flag 1 升序输出，-1降序输出
    ************************************************************************/
    long PrepareGetData(long flag)
    {
        if(!((m_fDelSearch == NULL || m_fDelSearch->GetDataNum()==0) && m_fSearch->GetDataNum() == 0))
        {
            if(ArrHash() < 0)
                return -1;
        }
        m_lOutFlag = flag;
        if(flag>0)
        {
            m_lOutIndex = 0;
            m_lOutOff = 0;
        }
        else
        {
            m_lOutIndex = m_lNowIndex-1;
            m_lOutOff = m_lLastPageNum-1;
        }
        return 0;
    };
    /************************************************************************/
    /* 得到一条数据，用于数据顺序输出                                                                     */
    /************************************************************************/
    long GetData(T1 &key,void *&data)
    {
        if(m_lOutFlag == 0)
        {
            if(m_lOutOff < 0 || m_lOutIndex < 0)
                return -1;
            key = *(T1*)(m_pBlockIndex[m_lOutIndex]+m_lOutOff*m_lOneBlock);
            data = (void *)(m_pBlockIndex[m_lOutIndex]+m_lOutOff*m_lOneBlock+sizeof(T1));
            m_lOutOff--;
            if(m_lOutOff < 0)
            {
                m_lOutOff = m_lBlockSize-1;
                m_lOutIndex--;
            }
        }
        else
        {
            if(m_lOutIndex >= m_lNowIndex)
                return -1;
            key = *(T1*)(m_pBlockIndex[m_lOutIndex]+m_lOutOff*m_lOneBlock);
            data = (void *)(m_pBlockIndex[m_lOutIndex]+m_lOutOff*m_lOneBlock+sizeof(T1));
            m_lOutOff++;
            if(m_lOutIndex == m_lNowIndex-1)
            {
                if(m_lOutOff >= m_lLastPageNum)
                {
                    m_lOutIndex++;
                    m_lOutOff = 0;
                }
            }
            else
            {
                if(m_lOutOff >= m_lBlockSize)
                {
                    m_lOutIndex++;
                    m_lOutOff = 0;
                }
            }
        }
        return 0;
    };
    /************************************************************************/
    /* 删除一个键值                                                                     */
    /************************************************************************/
    long DelKey(T1 key)
    {
        void *vpData;
        if(Search(key,vpData) != 0)
            return -1;
        if(m_fDelSearch==NULL)
            m_fDelSearch = new FewSearch<T1>(m_lBlockSize*sizeof(T1),m_lBlockSize+1,0);
        m_fDelSearch->add(key,vpData,vpData,0);
        m_lNowData --;
        if(m_fDelSearch->GetDataNum() >= m_lBlockSize)
            ArrHash();
        return 0;
    };
    /************************************************************************/
    /* 函数功能：向系统中增加一条数据，用于数据域的长度为定值时
    * 参数：key 用于查找的键值 data 增加的数据域地址 old如果该键值已经存在，存放原来的数据域地址
    * 返回值：0 成功插入 -1键值已经存在  其他负值 出错，可能是不能分配内存
    ************************************************************************/
    long add(T1 key,void *data,void *&old)
    {
        void *vpData;
        long lDelFlag = 0;
        if(m_fSearch->GetDataNum() >= m_lBlockSize)
        {
            if(m_lNowIndex >= m_lMaxBlockNum)
            {
                old = NULL;
                return -2;
            }
            if(ArrHash() < 0)
            {
                old = NULL;
                return -3;
            }
        }
        if(m_fDelSearch && m_fDelSearch->Search(key,vpData) == 0)
        {
            m_fDelSearch->DeleteKey(key);
            lDelFlag = 1;
            m_lNowData++;
        }
        if(m_lNowIndex > 0 && SearchFromBigArray(key,vpData) == 0)
        {
            if(lDelFlag == 1)
            {
                memcpy(vpData,data,m_lContSize);
                return 0;
            }
            old = vpData;
            return -1;
        }
        long lRet;
        lRet = m_fSearch->add(key,data,old) ;
        if(lRet < 0)
        {
            if(lRet == -1 && lDelFlag == 1)
            {
                memcpy(old,data,m_lContSize);
                return 0;
            }
            return lRet;
        }
        m_lNowData ++;
        return 0;
    };
    /************************************************************************/
    /* 函数功能：查找
    * 输入参数：key 要查找的键值 data 返回的数据域指针
    * 返回值：0成功，-1 失败
    ************************************************************************/
    long Search(T1 key,void *&data)
    {
        if(m_fDelSearch && 0 == m_fDelSearch->Search(key,data))
            return -1;
        if(m_lNowIndex > 0 && SearchFromBigArray(key,data) == 0)
            return 0;
        return m_fSearch->Search(key,data);
    };
    /************************************************************************/
    /* 系统初始化，数据被清空
    ************************************************************************/
    void clear()
    {
        m_lNowData = 0;
        m_lNowIndex = 0;
        m_fSearch->clear();
    };
    /************************************************************************/
    /* 函数功能：返回当前类中存放的数据量
    ************************************************************************/
    long GetDataNum() {
        return m_lNowData;
    };
    /************************************************************************/
    /* 函数功能：将内存中的树保存到文件中                                                                     */
    /************************************************************************/
    long SaveTree( const char* pFileName)
    {
        char filename[512];
        sprintf(filename,"%s_bak",pFileName);
        FILE* fp = fopen(filename, "wb");
        if(fp == NULL)
            return -1;
        PrepareGetData(1);
        T1 key;
        void* data;
        while(GetData(key, data) != -1)
        {
            if(fwrite(&key, sizeof(key), 1, fp) != 1)
            {
                fclose(fp);
                return -1;
            }
            if(m_lContSize > 0)
            {
                if(fwrite(data, m_lContSize, 1, fp) != 1)
                {
                    fclose(fp);
                    return -1;
                }
            }
        }
        fclose(fp);
        remove(pFileName);
        rename(filename,pFileName);
        return 0;
    }
    /************************************************************************/
    /* 函数功能：加载文件中的数据                                                                      */
    /************************************************************************/
    long LoadTree( const char* pFileName,BigSearch * Tree)
    {
        FILE* fp = fopen(pFileName, "rb");
        if(fp == NULL)
            return -1;
        T1 key;
        void * pOld;
        unsigned char * data;
        //char aaa;
        if(m_lContSize > 0)
        {
            data = new unsigned char[m_lContSize];
        }
        while(fread(&key, sizeof(key), 1, fp) == 1)
        {
            if(Tree !=NULL)
            {
                if(Tree->Search(key,pOld) ==0)
                    continue;
            }
            if(m_lContSize > 0)
            {
                if(fread(data, m_lContSize, 1, fp) != 1)
                {
                    fclose(fp);
                    return -1;
                }
                //aaa = (signed char)(data[320]);
                //printf("aaa = %c\n",aaa);
                add(key, data, pOld);
            }
            else
            {
                add(key, NULL, pOld);
            }
        }
        fclose(fp);
        return 0;
    }
    /************************************************************************/
    /*                                                  */
    /************************************************************************/

private:

    long ArrHash()
    {
        char *p1;
        char *p2;
        long i,j,k;
        if(m_lNowData == 0)
        {
            for(i=0; i<m_lNowIndex; i++)
            {
                delete m_pBlockIndex[i];
                m_pBlockIndex[i] = NULL;
            }
            m_fSearch->clear();
            if(m_fDelSearch)
                m_fDelSearch->clear();
            m_lNowIndex = 0;
            m_lLastPageNum = 0;
            return 0;
        }
        if(m_lNowIndex >= m_lMaxBlockNum)
            return -1;
        if(m_fSearch->GetDataNum() > m_lBlockSize)
            return -1;
        long lArrNum = m_fSearch->GetFixLenData(m_pBlockIndex[m_lMaxBlockNum],m_lBlockSize,1);
        if(m_pBlockIndex[m_lNowIndex] == NULL)
        {
            m_pBlockIndex[m_lNowIndex] = new char [m_lOneBlock * m_lBlockSize];
            if(m_pBlockIndex[m_lNowIndex] ==NULL)
            {
                printf("开辟内存失败\n");
                return -1;
            }
        }
        k = 0;
        long lWriteIndex = m_lNowIndex;
        long lWriteOff = 0;
        char *pOff = m_pBlockIndex[m_lMaxBlockNum];
        long lLoopIndex;
        void *vpData;
        long lCompareFlag ;
        long lSearchFlagArr = 0,lSearchFlagHash=0;
        for(i = 0; i < m_lNowIndex; i++)
        {
            if(i == m_lNowIndex - 1)
                lLoopIndex = m_lLastPageNum;
            else
                lLoopIndex = m_lBlockSize;
            for(j = 0; j < lLoopIndex;)
            {
                p1 = m_pBlockIndex[i] + j * m_lOneBlock;
                if(lSearchFlagArr == 0 && m_fDelSearch && 0 == m_fDelSearch->Search(*(T1 *)p1,vpData))
                {
                    j++;
                    continue;
                }
                lSearchFlagArr = 1;
                if(k < lArrNum)
                {
                    p2 = pOff + k * m_lOneBlock;
                    if(lSearchFlagHash == 0 && m_fDelSearch && 0 == m_fDelSearch->Search(*(T1 *)p2,vpData))
                    {
                        k++;
                        continue;
                    }
                    if(*(T1 *)p1 < *(T1 *)p2)
                        lCompareFlag = 0;
                    else
                        lCompareFlag = 1;
                }
                else
                    lCompareFlag = 0;
                if(lCompareFlag == 0)
                {
                    memcpy(m_pBlockIndex[lWriteIndex] + lWriteOff * m_lOneBlock,p1,m_lOneBlock);
                    lWriteOff++;
                    if(lWriteOff >= m_lBlockSize)
                    {
                        if(lWriteIndex == m_lNowIndex)
                            lWriteIndex = 0;
                        else
                            lWriteIndex ++ ;
                        lWriteOff = 0;
                    }
                    j ++;
                    lSearchFlagHash = 1;
                }
                else
                {
                    memcpy(m_pBlockIndex[lWriteIndex] + lWriteOff * m_lOneBlock,p2,m_lOneBlock);
                    lWriteOff++;
                    if(lWriteOff >= m_lBlockSize)
                    {
                        lWriteOff = 0;
                        if(lWriteIndex == m_lNowIndex)
                            lWriteIndex = 0;
                        else
                            lWriteIndex ++ ;
                    }
                    k++;
                    lSearchFlagArr = 1;
                }
            }
        }
        for(; k<lArrNum; k++)
        {
            p2 = pOff + k * m_lOneBlock;
            if(m_fDelSearch && 0 == m_fDelSearch->Search(*(T1 *)p2,vpData))
                continue;
            memcpy(m_pBlockIndex[lWriteIndex] + lWriteOff * m_lOneBlock,p2,m_lOneBlock);
            lWriteOff++;
            if(lWriteOff >= m_lBlockSize)
            {
                lWriteOff = 0;
                if(lWriteIndex == m_lNowIndex)
                    lWriteIndex = 0;
                else
                    lWriteIndex ++ ;
            }
        }
        if(lWriteOff == 0)
        {
            m_lLastPageNum = m_lBlockSize;
        }
        else
            m_lLastPageNum = lWriteOff;
        if(m_lNowIndex == 0)//原来系统中没有
            m_lNowIndex = 1;
        else if(lWriteIndex == m_lNowIndex && lWriteOff != 0)//系统删除后只剩下一个点
        {
            if(m_pBlockIndex[0])
                delete m_pBlockIndex[0];
            m_pBlockIndex[0] = m_pBlockIndex[m_lNowIndex];
            m_pBlockIndex[m_lNowIndex] = NULL;
            for(k=1; k<m_lNowIndex; k++)
            {
                delete m_pBlockIndex[k];
                m_pBlockIndex[k] = NULL;
            }
            m_lNowIndex = 1;
        }
        else
        {
            char *p = m_pBlockIndex[m_lNowIndex];
            for(i=m_lNowIndex; i>0; i--)
                m_pBlockIndex[i] = m_pBlockIndex[i-1];
            m_pBlockIndex[0] = p;
            if(lWriteOff == 0)
                k = lWriteIndex+1;
            else
                k = lWriteIndex+2;

            for(i = k; i<=m_lNowIndex; i++)
            {
                delete m_pBlockIndex[i];
                m_pBlockIndex[i] = NULL;
            }
            m_lNowIndex = k;
        }
        m_fSearch->clear();
        if(m_fDelSearch)
            m_fDelSearch->clear();
        return 0;
    };
    long SearchFromBigArray(T1 key,void *&data)
    {
        long left=0,right=m_lNowIndex-1,mid;
        T1 tMidValue;
        long lGetIndex = -1;
        long lMax;
        while(left <= right)
        {
            mid = (left + right)>>1;
            tMidValue = *(T1*)m_pBlockIndex[mid];
            if(tMidValue > key)
                right = mid - 1;
            else if(tMidValue < key)
            {
                if(mid == m_lNowIndex-1)
                    lMax = m_lLastPageNum - 1;
                else
                    lMax = m_lBlockSize-1;
                if(*(T1*)(m_pBlockIndex[mid]+lMax*m_lOneBlock) >= key)
                {
                    lGetIndex = mid;
                    break;
                }
                left = mid + 1;
            }
            else
            {
                lGetIndex = mid;
                break;
            }
        }
        if(lGetIndex < 0) return -1;
        left = 0;
        if(lGetIndex == m_lNowIndex-1)
            right = m_lLastPageNum - 1;
        else
            right = m_lBlockSize - 1;


        while(left <= right)
        {
            mid = (left + right)>>1;
            tMidValue = *(T1*)(m_pBlockIndex[lGetIndex] + mid * m_lOneBlock);
            if(tMidValue > key)
                right = mid - 1;
            else if(tMidValue < key)
                left = mid + 1;
            else
            {
                data = (void *)(m_pBlockIndex[lGetIndex] + mid * m_lOneBlock + sizeof(T1));
                return 0;
            }
        }
        return -1;
    };
private:
    long m_lBlockSize;			//每一块内存存放的数据量
    long m_lMaxBlockNum;		//最大可开辟内存块数
    long m_lContSize;			//用户数据段的长度
    FewSearch<T1> * m_fSearch;	//存放小量查询数据
    char **m_pBlockIndex;		//大块数据的索引
    long m_lNowIndex;			//当前用到的内存块数
    long m_lNowData;			//系统当前的数据量
    long m_lOneBlock;			//单项数据大小（sizeof(T)+m_lContSize)
    long m_lOutIndex;				//用于遍历，当前遍历到的内存块
    long m_lOutOff;				//用于遍历，当前遍历到的内存偏移数
    int m_lOutFlag;				//遍历顺序，1 从小到大 0 从大到小
    FewSearch<T1>*m_fDelSearch;	//用于存放已经删除的键值
    long m_lLastPageNum;			//最后一块内存中已经存放的数据量
};

#endif
