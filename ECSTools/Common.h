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

#define OBJECT_CONFIG_PATH    "../ObjectConfig"
#define OBJECT_OUTPUT_PATH    "../ECSObject"
#define OBJECT_BASETYPE_PATH  "../ObjectConfig/BaseType.xml"
#define OBJECT_BASETYPE_FILE  "BaseType.h"
#define OBJECT_BASECLASS_FILE "BaseObject.h"

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

    _ObjectClass()
    {
        m_nBuffSize  = 0;
        m_nClassType = 0;
    }

    vec_Object_Info m_vec_Object_Info;    //变量列表
};

typedef vector<_ObjectClass> vec_ObjectClass;   //类对象结构

typedef vector<string> vec_Xml_File_Name;       //所有XML文件的路径

struct _BaseType
{
    string m_strBaseTypeName;            //typedef定义的名字
    string m_strTypeName;                //typedef定义的原始数据类型
    string m_strClassName;               //是单个对象，还是数据对象
    int    m_nLen;                       //对象总长度

    _BaseType()
    {
        m_nLen = 0;
    }
};

typedef vector<_BaseType> vec_Base_Type_List;

//验证对象是不是已定义的类型之一
static bool Check_Base_Type(string strType, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strBaseTypeName == strType)
        {
            return true;
        }
    }

    return false;
}

static int Get_Base_Type_Size(string strType, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strBaseTypeName == strType)
        {
            return obj_vec_Base_Type_List[i].m_nLen;
        }
    }

    return 0;
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

//创建Obect基类
static bool Create_Base_Class_H()
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    sprintf(szHFileName, "%s//%s", OBJECT_OUTPUT_PATH, OBJECT_BASECLASS_FILE);
    FILE* pFile = fopen(szHFileName, "w");

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create BaseObject.h\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _BASEOBJECT_H\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _BASEOBJECT_H\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class IObject\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual ~IObject() = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual bool Get_Stream(char* pData, int& nLen) = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual bool Set_Stream(char* pData, int& nLen) = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    fclose(pFile);
    return true;
}

//创建基本类型定义头文件
static bool Create_Base_Type_H(vec_Base_Type_List& obj_vec_Base_Type_List)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    sprintf(szHFileName, "%s//%s", OBJECT_OUTPUT_PATH, OBJECT_BASETYPE_FILE);
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[Create_Base_Type_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create BaseType.h\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _BASETYPE_H\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _BASETYPE_H\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <string>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <vector>\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "using namespace std;\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strClassName == "single")
        {
            //单个对象
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef %s %s;\n",
                         obj_vec_Base_Type_List[i].m_strTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
        else
        {
            //数组
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef %s %s[%d];\n",
                         obj_vec_Base_Type_List[i].m_strTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_nLen);
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //创建消息类型，用于各个类的参数调用
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\nstruct _Object_Info\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strName;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strType;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strMin;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strMax;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strInit;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nSize;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nStartPos;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef vector<_Object_Info> vec_Object_Info_List;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    fclose(pFile);
    return true;
}

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
