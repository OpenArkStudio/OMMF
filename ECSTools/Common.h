#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include "XmlOpeation.h"

using namespace std;

#define OBJECTCONFIG_PATH  "../ObjectConfig"

struct _Object_Info
{
    string m_strName;            //当前变量名
    string m_Type;               //当前变量类型
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

#endif
