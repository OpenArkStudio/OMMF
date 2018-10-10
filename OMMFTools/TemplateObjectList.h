#ifndef _TAMPLATEOBJECT_H
#define _TAMPLATEOBJECT_H

#include "BaseObject.h"
#include <time.h>


#define GUID_SIZE 24

//创建一个ObjectID
static int Create_Object_UID(char* pObjectUID, int& nLen, int nType, int nPos)
{
    if (nLen < GUID_SIZE)
    {
        //给的GUID字符串内存不够
        printf("[Create_Object_UID]no enough GUID_SIZE(%d).\n", nLen);
        return -1;
    }

    //创建规则，时间+ClassID+数组下标
    time_t ttCurrentTime;
    time(&ttCurrentTime);
    struct tm tmTime;
#ifdef WIN32
    localtime_s(&tmTime, &ttCurrentTime);
#else
    localtime_r(&ttCurrentTime, &tmTime);
#endif
    char szTimeNow[50] = { '\0' };
    sprintf(szTimeNow, "%04d%02d%02d%02d%02d%02d",
            tmTime.tm_year + 1900,
            tmTime.tm_mon + 1,
            tmTime.tm_mday,
            tmTime.tm_hour,
            tmTime.tm_min,
            tmTime.tm_sec);

    sprintf(pObjectUID, "%s%04d%06d", szTimeNow, nType, nPos);
    nLen = GUID_SIZE;
    return 0;
}

//从一个ObjectUID中获得当前类型ID
static int Get_Object_UID_Info(char* pObjectUID, int nLen, int& nType, int& nPos)
{
    if (GUID_SIZE != nLen)
    {
        printf("[Get_Object_UID_Info]error GUID_SIZE(%d).\n", nLen);
        return -1;
    }

    char szType[10] = { '\0' };
    char szPos[10] = { '\0' };

    memcpy(szType, &pObjectUID[14], 4);
    memcpy(szPos, &pObjectUID[18], 6);

    nType = atoi(szType);
    nPos = atoi(szPos);

    return 0;
}

//对应对象的槽位
struct _Object_Data_Solt
{
    IObject*  m_pObject;            //指针对象
    int       m_nState;             //当前指针状态，0：不可用，1未使用，2正在使用
    char      m_szUUID[GUID_SIZE];  //当前对象的唯一ID

    _Object_Data_Solt()
    {
        Init();
    }

    void Init()
    {
        m_pObject              = NULL;
        m_nState               = 0;
        m_szUUID[0]            = '\0';
    }

    void Set_UUID(const char* pUUID)
    {
        sprintf(m_szUUID, "%s", pUUID);
    }

    void Clear()
    {
        m_nState = 1;
        m_szUUID[0] = '\0';
    }
};

template <class T>
class COjectList
{
public:
    COjectList()
    {
        m_nType      = 0;
        m_nCount     = 0;
        m_objectList = NULL;

        Create_Object_UID_Fn = Create_Object_UID;
        Get_Object_UID_Info_Fn = Get_Object_UID_Info;
    };

    ~COjectList()
    {
        Close();
    };

    void Close()
    {
        delete[] m_objectList;
        m_objectList = NULL;
    }

    int Init(int nType, int nCount)
    {
        Close();

        m_nType    = nType;
        m_nCount   = nCount;

        m_objectList = new _Object_Data_Solt[nCount];

        for (int i = 0; i < m_nCount; i++)
        {
            m_objectList[i].m_pObject = (IObject* )new T();
            m_objectList[i].m_nState  = 1;
        }

        m_nUsedCount = 0;
        m_nCurrIndex = 0;

        return 0;
    }

    //从内存池中获取一个空闲的对象
    T* Create(char* pObjectUID, int& nLen)
    {
        //从m_nCurrIndex往后找
        for (int i = m_nCurrIndex; i < m_nCount; i++)
        {
            if (m_objectList[i].m_nState == 1)
            {
                m_objectList[i].m_nState = 2; //标记位正在使用
                m_nCurrIndex = i;
                Create_Object_UID_Fn(pObjectUID, nLen, m_nType, i);
                m_objectList[i].Set_UUID(pObjectUID);   //在槽位记录当前的UUID
                return (T*)m_objectList[i].m_pObject;
            }
        }

        //如果寻找到末尾还没找到，从头开始找
        for (int i = 0; i < m_nCurrIndex; i++)
        {
            if (m_objectList[i].m_nState == 1)
            {
                m_objectList[i].m_nState = 2; //标记位正在使用
                m_nCurrIndex = i;
                Create_Object_UID_Fn(pObjectUID, nLen, m_nType, i);
                m_objectList[i].Set_UUID(pObjectUID);   //在槽位记录当前的UUID
                return (T*)m_objectList[i].m_pObject;
            }
        }

        //如果都没有找到
        printf("[Create]Object no can use object.\n");
        return NULL;
    }

    //归还一个正在使用的对象
    bool Delete(char* pObjectUID, int nLen, T* pt)
    {
        int nType = 0;
        int nPos  = 0;

        if (NULL == pt)
        {
            printf("[Delete]pObjectUID is NULL.\n");
            return false;
        }

        if (0 == Get_Object_UID_Info_Fn(pObjectUID, nLen, nType, nPos))
        {
            if (nType != m_nType || nPos < 0 || nPos >= m_nCount)
            {
                printf("[Delete]error nType=%d, nPos=%d.\n", nType, nPos);
                return false;
            }

            //回收对象
            m_objectList[nPos].Clear();
            return true;
        }
        else
        {
            return false;
        }
    }

    //找到一个指定的对象
    T* Get_Object(char* pObjectUID, int& nLen)
    {
        int nType = 0;
        int nPos = 0;

        if (0 == Get_Object_UID_Info_Fn(pObjectUID, nLen, nType, nPos))
        {
            if (nType != m_nType || nPos < 0 || nPos >= m_nCount)
            {
                printf("[Get_Object]error nType=%d, nPos=%d.\n", nType, nPos);
                return NULL;
            }

            if (2 == m_objectList[nPos].m_nState)
            {
                return (T*)m_objectList[nPos].m_pObject;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }

    //得到当前的缓冲池中的Count
    int Get_Count()
    {
        return m_nCount;
    }

private:
    int (*Create_Object_UID_Fn)(char* pObjectUID, int& nLen, int nType, int nPos);               //创建对象UID的消息函数指针
    int (*Get_Object_UID_Info_Fn)(char* pObjectUID, int nLen, int& nType, int& nPos);       //获得指定对象UID分类函数指针

    int                m_nType;           //当前的类对象类型
    int                m_nCount;          //当前缓冲数组对象的个数
    int                m_nUsedCount;      //当前正在使用对象的个数
    int                m_nCurrIndex;      //当前指针，用于Create使用
    _Object_Data_Solt* m_objectList;      //对象指针数组
};

#endif
