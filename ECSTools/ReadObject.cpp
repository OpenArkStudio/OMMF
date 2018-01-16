#include "ReadObject.h"

CReadObject::CReadObject()
{

}

CReadObject::~CReadObject()
{

}

bool CReadObject::WriteClass(int i, vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    //首先创建头文件
    if (false == Create_Object_H(i, objObjectClassList, obj_vec_Base_Type_List))
    {
        printf("[CReadObject::WriteClass]Create (%s) H file error.\n", objObjectClassList[i].m_strClassName.c_str());
    }

    return true;
}

bool CReadObject::Check_Type_In_Class(string strBaseTypeName, vec_Object_Info obj_vec_Object_Info)
{
    for (int i = 0; i < (int)obj_vec_Object_Info.size(); i++)
    {
        if (strBaseTypeName == obj_vec_Object_Info[i].m_strType)
        {
            return true;
        }
    }

    return false;
}

bool CReadObject::Create_Object_H(int nIndex, vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE]  = { '\0' };

    sprintf(szHFileName, "%s//%s.h", OBJECT_OUTPUT_PATH, objObjectClassList[nIndex].m_strClassName.c_str());
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_Object_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    //检验需要引用的其他类头文件
    for (int j = 0; j < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); j++)
    {
        _Object_Info& obj_Object_Info = objObjectClassList[nIndex].m_vec_Object_Info[j];

        if (false == Check_Base_Type(obj_Object_Info.m_strType, obj_vec_Base_Type_List))
        {
            printf("[CReadObject::Create_Object_H]no find base type(%s:%s) at [%s].\n",
                   obj_Object_Info.m_strName.c_str(),
                   obj_Object_Info.m_strType.c_str(),
                   objObjectClassList[nIndex].m_strClassName.c_str());
            return false;
        }
    }

    //添加头文件
    char szDefine[200] = { '\0' };
    To_Upper_String(objObjectClassList[nIndex].m_strClassName.c_str(), szDefine);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create %s\n\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _%s_H\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _%s_H\n\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s\"\n", OBJECT_BASETYPE_FILE);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class C%s\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tC%s();\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t~C%s();\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //创建所有的写BaseType方法
    for (int j = 0; j < (int)obj_vec_Base_Type_List.size(); j++)
    {
        if(true == Check_Type_In_Class(obj_vec_Base_Type_List[j].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(const char* pValueName, %s& Value);\n", obj_vec_Base_Type_List[j].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //创建所有的读BaseType方法
    for (int j = 0; j < (int)obj_vec_Base_Type_List.size(); j++)
    {
        if (true == Check_Type_In_Class(obj_vec_Base_Type_List[j].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Data(const char* pValueName, %s& Value);\n", obj_vec_Base_Type_List[j].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "private:\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);


    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    fclose(pFile);

    return true;
}


