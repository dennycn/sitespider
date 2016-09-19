#ifndef __MEMORY__MANAGE__HEAD__
#define __MEMORY__MANAGE__HEAD__
#include <stdio.h>
#include <string.h>
//#include <unistd.h>
//#include <pthread.h>


/*
 *	用于小块内存管理,初始化可以设置最大的内存开辟次数和一次开辟内存的大小
 *  所有使用的内存只能增加，不能释放，除非释放所有的内存
 */
class MemoryManage
{
public:
    /************************************************************************
    * 输入参数: lMaxBlockNum:可以开辟内存的次数
    *			 lBlockSize:  每一次开辟内存的大小
    ************************************************************************/
    MemoryManage(long lMaxBlockNum,long lBlockSize)
    {
        m_lNowIndex = 0;
        m_lNowOff = 0;
        m_lMaxIndex = lMaxBlockNum;
        m_pMallocBuf = new char *[m_lMaxIndex];
        memset(m_pMallocBuf,0,sizeof(char *) * m_lMaxIndex);
        m_pMallocBuf[0] = new char[lBlockSize];
        m_lOneBufSize = lBlockSize;
#ifdef _MT
        pthread_mutex_init(&cri, NULL);

#endif
    };
    ~MemoryManage()
    {
        for(long i = 0; i<m_lMaxIndex; i++)
        {
            if(m_pMallocBuf[i]) delete m_pMallocBuf[i];
        }
        delete m_pMallocBuf;
#ifdef _MT
        pthread_mutex_destroy(&cri);
#endif
    };
    /************************************************************************/
    /* 开辟内存，输入参数为要开辟内存的大小                                                                     */
    /************************************************************************/
    void *Alloc(long lLocSize)
    {
#ifdef _MT
        pthread_mutex_lock(&cri);
#endif
        char *p = NULL;
        if(m_lNowIndex >= m_lMaxIndex)return NULL;
        if(lLocSize > m_lOneBufSize || lLocSize <= 0)
        {
#ifdef _MT
            pthread_mutex_unlock(&cri);
#endif
            return NULL;
        }

        if(lLocSize + m_lNowOff > m_lOneBufSize)
        {
            m_lNowOff = 0;
            m_lNowIndex ++;
            if(m_lNowIndex >= m_lMaxIndex)
            {
#ifdef _MT
                pthread_mutex_unlock(&cri);
#endif
                return NULL;
            }
            if(m_pMallocBuf[m_lNowIndex] == NULL)
                m_pMallocBuf[m_lNowIndex] = new char[m_lOneBufSize];
        }
        p = m_pMallocBuf[m_lNowIndex] + m_lNowOff;
        m_lNowOff += lLocSize;
#ifdef _MT
        pthread_mutex_unlock(&cri);
#endif
        return (void *)p;
    };
    /************************************************************************/
    /* 功能：清除所有已经用过的内存，变为初始状态                                                                     */
    /************************************************************************/
    void clear()
    {
#ifdef _MT
        pthread_mutex_lock(&cri);
#endif
        m_lNowOff = 0;
        m_lNowIndex = 0;
#ifdef _MT
        pthread_mutex_unlock(&cri);
#endif
    };
private:
    long m_lMaxIndex;
    long m_lNowIndex;
    char **m_pMallocBuf;
    long m_lNowOff;
    long m_lOneBufSize;
#ifdef _MT
    CRITICAL_SECTION cri;
#endif
};
/*
 *	用于定长块内存管理,初始化可以设置最大的内存开辟次数和一次开辟内存的块个数，以及每个内存块的大小，最少4个字节
 *  所有使用的内存只能增加，不能释放，除非释放所有的内存
 */
class BlockMemoryManage
{
public:
    /************************************************************************
    * 输入参数: lMaxBlockNum:可以开辟内存的次数
    *			 lBlockSize:  块大小
    *			 lOneMemoryBlockNum:块大小
    ************************************************************************/
    BlockMemoryManage(long lMaxMemoryNum,long lOneMemoryDataNum,long lBlockSize)
    {
        pManage = new MemoryManage(lMaxMemoryNum,lBlockSize * lOneMemoryDataNum);
        pIdle = NULL;
        m_lOneBufSize = lBlockSize;
#ifdef _MT
        pthread_mutex_init(&cri);
#endif
    };
    ~BlockMemoryManage()
    {
        delete pManage;
#ifdef _MT
        pthread_mutex_destroy(&cri);
#endif
    };
    /************************************************************************/
    /* 开辟内存，输入参数为要开辟内存的大小                                                                     */
    /************************************************************************/
    void *Alloc()
    {
        char *p = NULL;
#ifdef _MT
        pthread_mutex_lock(&cri);
#endif
        if(pIdle)
        {
            p = pIdle;
            pIdle = *(char **)pIdle;
#ifdef _MT
            pthread_mutex_unlock(&cri);
#endif
            return (void *)p;
        }
#ifdef _MT
        pthread_mutex_unlock(&cri);
#endif
        return pManage->Alloc(m_lOneBufSize);
    };
    /************************************************************************
     功能：清除所有已经用过的内存，变为初始状态
    ************************************************************************/
    void clear()
    {
#ifdef _MT
        pthread_mutex_lock(&cri);
#endif
        pIdle = NULL;
        pManage->clear();
#ifdef _MT
        pthread_mutex_unlock(&cri);
#endif
    };
    /************************************************************************/
    /* 功能：释放内存                                                                    */
    /************************************************************************/
    void Free(void *pBuf)
    {
#ifdef _MT
        pthread_mutex_lock(&cri);
#endif
        *(char**)pBuf = pIdle;
        pIdle = (char*)pBuf;
#ifdef _MT
        pthread_mutex_unlock(&cri);
#endif
    };
private:
    MemoryManage *pManage;
    long m_lOneBufSize;
    char *pIdle;
#ifdef _MT
    pthread_mutex_t cri;
#endif
};
#endif

