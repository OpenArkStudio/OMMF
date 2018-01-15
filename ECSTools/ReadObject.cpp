#include "ReadObject.h"

CReadObject::CReadObject()
{

}

CReadObject::~CReadObject()
{

}

bool CReadObject::WriteClass(int i, vec_ObjectClass objObjectClassList)
{
    //首先创建头文件
    if (false == Create_Object_H(i, objObjectClassList))
    {
        printf("[CReadObject::WriteClass]Create (%s) H file error.\n", objObjectClassList[i].m_strClassName.c_str());
    }

    return true;
}

bool CReadObject::Create_Object_H(int i, vec_ObjectClass objObjectClassList)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[500]  = { '\0' };
    sprintf(szHFileName, "%s//%s.h", OBJECT_OUTPUT_PATH, objObjectClassList[i].m_strClassName.c_str());
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_Object_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    vector<string> obj_Vec_ClassName;

    //检验需要引用的其他类头文件
    for (int j = 0; j < (int)objObjectClassList[i].m_vec_Object_Info.size(); j++)
    {
        _Object_Info& obj_Object_Info = objObjectClassList[i].m_vec_Object_Info[j];

        if (obj_Object_Info.m_strType != "float" &&
            obj_Object_Info.m_strType != "int" &&
            obj_Object_Info.m_strType != "string")
        {
            //不是基本类型，去寻找类型定义
            if (false == Check_Object_exist(obj_Object_Info.m_strType, objObjectClassList))
            {
                printf("[CReadObject::Create_Object_H]no find class(%s) at xml.\n", obj_Object_Info.m_strType.c_str());
                return false;
            }

            obj_Vec_ClassName.push_back(obj_Object_Info.m_strType);
        }
    }

    //添加头文件
    char szDefine[200] = { '\0' };
    To_Upper_String(objObjectClassList[i].m_strClassName.c_str(), szDefine);
    sprintf_safe(szCodeLine, 200, "//create %s\n\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, 200, "#ifndef _%s_h\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, 200, "#define _%s_h\n\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, 200, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for(int j = 0; j < (int)obj_Vec_ClassName.size(); j++)
    {
        sprintf_safe(szCodeLine, 200, "#include \"%s.h\"\n", objObjectClassList[i].m_strClassName);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    fclose(pFile);

    return true;
}

bool CReadObject::Check_Object_exist(string strClassName, vec_ObjectClass objObjectClassList)
{
    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        if (strClassName == objObjectClassList[i].m_strClassName)
        {
            return true;
        }
    }

    return false;
}

