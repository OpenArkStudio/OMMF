#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <ctype.h>
#include<iostream>
#include<fstream>
#include <string>

#include "XmlOpeation.h"

using namespace std;

#define OBJECTCONFIG_PATH  "../ObjectConfig"
#define OBJECT_OUTPUT_PATH "../ECSObject"

#define MAX_CODE_LINE_SIZE 500

struct _Object_Info
{
    string m_strName;            //当前变量名
    string m_strType;            //当前变量类型
    string m_strMin;             //当前变量最小值
    string m_strMax;             //当前变量最大值
    string m_strInit;            //当前变量初始值
};

typedef vector<_Object_Info> vec_Object_Info;

struct _ObjectClass
{
    string m_strClassName;                //对象名
    int    m_nBuffSize;                   //对象最大缓冲大小
    int    m_nClassType;                  //对象类型ID

    vec_Object_Info m_vec_Object_Info;    //变量列表
};

typedef vector<_ObjectClass> vec_ObjectClass;

typedef vector<string> vec_Xml_File_Name;

typedef vector<string> vec_Base_Type;

static void Get_Base_Type(vec_Base_Type& obj_vec_Base_Type_List)
{
    obj_vec_Base_Type_List.clear();
    obj_vec_Base_Type_List.push_back("int");
    obj_vec_Base_Type_List.push_back("float");
    obj_vec_Base_Type_List.push_back("long");
    obj_vec_Base_Type_List.push_back("string");
}

static bool Check_Base_Type(string strType, vec_Base_Type& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i] == strType)
        {
            return true;
        }
    }

    return false;
}

static string Get_Type_Name(string strType, vec_Base_Type& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i] == strType)
        {
            return strType;
        }
    }

    return "C" + strType;
}

//安全的字符串赋值
static void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
    if (szText == NULL)
    {
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(szText, nLen, fmt, ap);
    szText[nLen - 1] = '\0';

    va_end(ap);
};

//字母小写转换大写
static void To_Upper_String(const char* pSrc, char* pRet)
{
    int nLen = (int)strlen(pSrc);

    for (int i = 0; i < nLen; i++)
    {
        pRet[i] = toupper(pSrc[i]);
    }

    pRet[nLen] = '\0';
}

//获得一个文件名
static string Get_File_From_Path(string strPath)
{
    char szPath[300]     = { '\0' };
    char szFileName[300] = { '\0' };

    sprintf_safe(szPath, 300, "%s", strPath.c_str());
    int nLen = (int)strlen(szPath);
    int nPos = 0;

    for (int i = nLen - 1; i >= 0; i--)
    {
        if ('\\' == szPath[i] || '/' == szPath[i])
        {
            nPos = i + 1;
            break;
        }
    }

    memcpy(szFileName, &szPath[nPos], nLen - nPos);
    szFileName[nLen - nPos] = '\0';

    //去掉文件末尾后缀
    for (int i = (int)strlen(szFileName); i >= 0; i--)
    {
        if ('.' == szFileName[i])
        {
            szFileName[i] = '\0';
            break;
        }
    }

    return (string)szFileName;
}

#endif
