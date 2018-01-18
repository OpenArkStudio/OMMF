#include "ReadObject.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

//生成工具，将指定的XML拆解成指定的Class对象，并提供Get Set方法。
//add by freeeyes

//遍历指定的目录，获得所有XML文件名
bool Read_Xml_Folder(string folderPath, vec_Xml_File_Name& obj_vec_Xml_File_Name)
{
#ifdef WIN32
    _finddata_t FileInfo;
    string strfind = folderPath + "\\*";
    long Handle = _findfirst(strfind.c_str(), &FileInfo);

    if (Handle == -1L)
    {
        return false;
    }

    do
    {
        //判断是否有子目录
        if (FileInfo.attrib & _A_SUBDIR)
        {
            //这个语句很重要
            if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
            {
                //不必支持子目录遍历
                //string newPath = folderPath + "\\" + FileInfo.name;
                //dfsFolder(newPath);
            }
        }
        else
        {
            if (strcmp(FileInfo.name, "BaseType.xml") != 0)
            {
                string filename = folderPath + "\\" + FileInfo.name;
                obj_vec_Xml_File_Name.push_back(filename);
            }
        }
    }
    while (_findnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
#else
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;

    if ((dp = opendir(folderPath.c_str())) == NULL)
    {
        printf("cannot open directory: %s\n", folderPath.c_str());
        return false;
    }

    chdir(folderPath.c_str());

    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);

        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            {
                continue;
            }

            //不需要支持子目录遍历
            //printf("%*s%s/\n",depth,"",entry->d_name);
            //dfsFolder(entry->d_name,depth+4);
        }
        else
        {
            if (strcmp("BaseType.xml", entry->d_name) != 0)
            {
                string filename = folderPath + "/" + entry->d_name;
                obj_vec_Xml_File_Name.push_back(filename);
            }
        }
    }

    chdir("..");
    closedir(dp);
#endif
    return true;
}

//读取基本类型声明表
bool Read_Base_Type_XML_File(vec_Base_Type_List& obj_vec_Base_Type_List)
{
    CXmlOpeation obj_MainConfig;

    obj_vec_Base_Type_List.clear();

    if (false == obj_MainConfig.Init(OBJECT_BASETYPE_PATH))
    {
        printf("[Read_Base_Type_XML_File]File Read Error = %s.\n", OBJECT_BASETYPE_PATH);
        return false;
    }

    //解析类相关参数信息
    char* pData = NULL;

    TiXmlElement* pNextTiXmlElementName  = NULL;
    TiXmlElement* pNextTiXmlElementType  = NULL;
    TiXmlElement* pNextTiXmlElementClass = NULL;
    TiXmlElement* pNextTiXmlElementSize  = NULL;

    while (true)
    {
        _BaseType obj_BaseType;
        pData = obj_MainConfig.GetData("CObject", "name", pNextTiXmlElementName);

        if (pData != NULL)
        {
            obj_BaseType.m_strBaseTypeName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "type", pNextTiXmlElementType);

        if (pData != NULL)
        {
            obj_BaseType.m_strTypeName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "class", pNextTiXmlElementClass);

        if (pData != NULL)
        {
            obj_BaseType.m_strClassName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "size", pNextTiXmlElementSize);

        if (pData != NULL)
        {
            obj_BaseType.m_nLen = atoi(pData);
        }
        else
        {
            break;
        }


        obj_vec_Base_Type_List.push_back(obj_BaseType);
    }

    return true;
}


//遍历读取XML文件，还原类结构
bool Read_XML_File(vec_Xml_File_Name& obj_vec_Xml_File_Name, vec_ObjectClass& obj_vec_ObjectClass)
{
    obj_vec_ObjectClass.clear();

    for (int i = 0; i < (int)obj_vec_Xml_File_Name.size(); i++)
    {
        printf("[Read_XML_File]filename=%s.\n", obj_vec_Xml_File_Name[i].c_str());

        _ObjectClass obj_ObjectClass;
        CXmlOpeation obj_MainConfig;

        if (false == obj_MainConfig.Init(obj_vec_Xml_File_Name[i].c_str()))
        {
            printf("[Read_XML_File]File Read Error = %s.\n", obj_vec_Xml_File_Name[i].c_str());
            return false;
        }

        //解析类相关参数信息
        char* pData = NULL;
        obj_ObjectClass.m_strClassName = Get_File_From_Path(obj_vec_Xml_File_Name[i]);
        pData = obj_MainConfig.GetData("ClassInfo", "BuffSize");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nBuffSize = atoi(pData);
        }

        pData = obj_MainConfig.GetData("ClassInfo", "ClassType");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nClassType = atoi(pData);
        }

        TiXmlElement* pNextTiXmlElementName = NULL;
        TiXmlElement* pNextTiXmlElementType = NULL;
        TiXmlElement* pNextTiXmlElementMin  = NULL;
        TiXmlElement* pNextTiXmlElementMax  = NULL;
        TiXmlElement* pNextTiXmlElementInit = NULL;

        while (true)
        {
            _Object_Info obj_Object_Info;
            pData = obj_MainConfig.GetData("CObject", "name", pNextTiXmlElementName);

            if (pData != NULL)
            {
                obj_Object_Info.m_strName = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "type", pNextTiXmlElementType);

            if (pData != NULL)
            {
                obj_Object_Info.m_strType = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "min", pNextTiXmlElementMin);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMin = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMin = "";
            }

            pData = obj_MainConfig.GetData("CObject", "max", pNextTiXmlElementMax);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMax = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMax = "";
            }

            pData = obj_MainConfig.GetData("CObject", "init", pNextTiXmlElementInit);

            if (pData != NULL)
            {
                obj_Object_Info.m_strInit = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strInit = "";
            }

            obj_ObjectClass.m_vec_Object_Info.push_back(obj_Object_Info);
        }

        obj_vec_ObjectClass.push_back(obj_ObjectClass);


    }

    return true;
}

int main()
{
    bool blRet = false;
    vec_Xml_File_Name obj_vec_Xml_File_Name;
    vec_ObjectClass  obj_vec_ObjectClass;
    vec_Base_Type_List obj_vec_Base_Type_List;

    //获得基础类型名
    blRet = Read_Base_Type_XML_File(obj_vec_Base_Type_List);

    if (false == blRet)
    {
        printf("[Main]Read_Base_Type_XML_File is fail.\n");
        return 0;
    }

    //获得所有的xml文件名
    blRet = Read_Xml_Folder(OBJECT_CONFIG_PATH, obj_vec_Xml_File_Name);

    if (false == blRet)
    {
        printf("[Main]Read_Xml_Folder is fail.\n");
        return 0;
    }

    //将所有的xml转化成ObjectClass描述数据结构
    blRet = Read_XML_File(obj_vec_Xml_File_Name, obj_vec_ObjectClass);

    if (false == blRet)
    {
        printf("[Main]Read_XML_File is fail.\n");
        return 0;
    }

    //创建公共头文件
    Create_Base_Type_H(obj_vec_Base_Type_List);

    //创建基类文件
    Create_Base_Class_H();

    //开始生成文件
    CReadObject objReadObject;

    for (int i = 0; i < (int)obj_vec_ObjectClass.size(); i++)
    {
        objReadObject.WriteClass(i, obj_vec_ObjectClass, obj_vec_Base_Type_List);
    }

    getchar();
    return 0;
}
