#ifndef __MEMORY__MANAGE__HEAD__
#define __MEMORY__MANAGE__HEAD__
#include <stdio.h>
#include <string.h>
//#include <unistd.h>
//#include <pthread.h>


/*
 *	����С���ڴ����,��ʼ���������������ڴ濪�ٴ�����һ�ο����ڴ�Ĵ�С
 *  ����ʹ�õ��ڴ�ֻ�����ӣ������ͷţ������ͷ����е��ڴ�
 */
class MemoryManage
{
public:
    /************************************************************************
    * �������: lMaxBlockNum:���Կ����ڴ�Ĵ���
    *			 lBlockSize:  ÿһ�ο����ڴ�Ĵ�С
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
    /* �����ڴ棬�������ΪҪ�����ڴ�Ĵ�С                                                                     */
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
    /* ���ܣ���������Ѿ��ù����ڴ棬��Ϊ��ʼ״̬                                                                     */
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
 *	���ڶ������ڴ����,��ʼ���������������ڴ濪�ٴ�����һ�ο����ڴ�Ŀ�������Լ�ÿ���ڴ��Ĵ�С������4���ֽ�
 *  ����ʹ�õ��ڴ�ֻ�����ӣ������ͷţ������ͷ����е��ڴ�
 */
class BlockMemoryManage
{
public:
    /************************************************************************
    * �������: lMaxBlockNum:���Կ����ڴ�Ĵ���
    *			 lBlockSize:  ���С
    *			 lOneMemoryBlockNum:���С
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
    /* �����ڴ棬�������ΪҪ�����ڴ�Ĵ�С                                                                     */
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
     ���ܣ���������Ѿ��ù����ڴ棬��Ϊ��ʼ״̬
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
    /* ���ܣ��ͷ��ڴ�                                                                    */
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

