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
    char szCodeLine[MAX_CODE_LINE_SIZE]  = { '\0' };
    vec_Base_Type obj_vec_Base_Type_List;

    Get_Base_Type(obj_vec_Base_Type_List);

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

        if (false == Check_Base_Type(obj_Object_Info.m_strType, obj_vec_Base_Type_List))
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
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create %s\n\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _%s_h\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _%s_h\n\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for(int j = 0; j < (int)obj_Vec_ClassName.size(); j++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s.h\"\n", obj_Vec_ClassName[j].c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class C%s\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tC%s();\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t~C%s();\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, int& nValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, float& fValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, long& lValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, char* pValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, int& nValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Data(const char* pValueName, float& fValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Data(const char* pValueName, long& fValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Data(const char* pValueName, char* pValue);\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "private:\n", objObjectClassList[i].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int j = 0; j < (int)objObjectClassList[i].m_vec_Object_Info.size(); j++)
    {
        _Object_Info& obj_Object_Info = objObjectClassList[i].m_vec_Object_Info[j];
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t%s %s;\n", Get_Type_Name(obj_Object_Info.m_strType, obj_vec_Base_Type_List).c_str(), obj_Object_Info.m_strName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

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

