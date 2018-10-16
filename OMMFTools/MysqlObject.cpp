#include "MysqlObject.h"

CMysqlObject::CMysqlObject()
{
}

CMysqlObject::~CMysqlObject()
{
}

bool CMysqlObject::WriteMysqlCode(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info)
{
    //这里根据对应的xml生成mysql读写代码
    if (false == Create_Mysql_Code_H(objMysqlDBList, objObjectList, obj_Base_Type_List_info))
    {
        return false;
    }

    if (false == Create_Mysql_Code_Cpp(objMysqlDBList, objObjectList, obj_Base_Type_List_info))
    {
        return false;
    }

    if (false == Create_Mysql_Script(objMysqlDBList, objObjectList, obj_Base_Type_List_info))
    {
        return false;
    }

    return true;
}

_ObjectClass* CMysqlObject::Get_ObjectClass(int nClassID, vec_ObjectClass& objObjectList)
{
    _ObjectClass* pObjectClass = NULL;

    for (int i = 0; i < (int)objObjectList.size(); i++)
    {
        if (objObjectList[i].m_nClassID == nClassID)
        {
            pObjectClass = &objObjectList[i];
            return pObjectClass;
        }
    }

    return NULL;
}

int CMysqlObject::Get_Object_Size(string strName, _Base_Type_List_info obj_Base_Type_List_info)
{
    for (int i = 0; i < (int)obj_Base_Type_List_info.m_vec_Base_Type_List.size(); i++)
    {
        if (obj_Base_Type_List_info.m_vec_Base_Type_List[i].m_strBaseTypeName == strName)
        {
            return obj_Base_Type_List_info.m_vec_Base_Type_List[i].m_nSaveSize;
        }
    }

    return 1;
}

bool CMysqlObject::Create_Mysql_Code_H(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };
    _ObjectClass* pObjectClass = NULL;

    //创建对应目录
    if (Create_Project_Path(MYSQL_OUTPUT_PATH) == false)
    {
        return false;
    }

    //遍历寻找需要的mysql化的类
    for (int i = 0; i < (int)objMysqlDBList.size(); i++)
    {
        for (int j = 0; j < (int)objMysqlDBList[i].m_vec_Xml_Mysql_Table.size(); j++)
        {
            pObjectClass = NULL;
            pObjectClass = Get_ObjectClass(objMysqlDBList[i].m_vec_Xml_Mysql_Table[j].m_nClassID, objObjectList);

            if (NULL != pObjectClass)
            {
                //开始创建头文件
                sprintf_safe(szHFileName, 200, "%s/Mysql_%s.h", MYSQL_OUTPUT_PATH, pObjectClass->m_strClassName.c_str());
                FILE* pFile = fopen(szHFileName, "w");
                char szDefine[200] = { '\0' };
                To_Upper_String(pObjectClass->m_strClassName.c_str(), szDefine);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _MYSQL_%s_H\n", szDefine);
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _%s_H\n\n", szDefine);
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <mysql.h>\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"OMMFObject\\%s.h\"\n", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"OMMFObject\\ObjectManager.h\"\n\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int Load_%s_Data(MYSQL* pConn, vector<_Object_Data_Solt*>& vecObjectList);\n", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int Save_%s_Data(MYSQL* pConn, vector<_Object_Data_Solt*>& vecObjectList);\n", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n", szDefine);
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                fclose(pFile);
            }
        }
    }


    return true;
}

bool CMysqlObject::Create_Mysql_Code_Cpp(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };
    _ObjectClass* pObjectClass = NULL;

    //创建对应目录
    if (Create_Project_Path(MYSQL_OUTPUT_PATH) == false)
    {
        return false;
    }

    //遍历寻找需要的mysql化的类
    for (int i = 0; i < (int)objMysqlDBList.size(); i++)
    {
        for (int j = 0; j < (int)objMysqlDBList[i].m_vec_Xml_Mysql_Table.size(); j++)
        {
            pObjectClass = NULL;
            pObjectClass = Get_ObjectClass(objMysqlDBList[i].m_vec_Xml_Mysql_Table[j].m_nClassID, objObjectList);

            if (NULL != pObjectClass)
            {
                //开始创建头文件
                sprintf_safe(szHFileName, 200, "%s/Mysql_%s.cpp", MYSQL_OUTPUT_PATH, pObjectClass->m_strClassName.c_str());
                FILE* pFile = fopen(szHFileName, "w");
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s/Mysql_%s.h\"\n\n", MYSQL_OUTPUT_PATH, pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int Load_%s_Data(MYSQL* pConn, vector<_Object_Data_Solt*>& vecObjectList);\n", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tchar szSQL[2048] = { '\\0' };\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tsprintf(szSQL, \"select GUID, ");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s, ", pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s from tb_%s\");\n\n",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                     pObjectClass->m_strClassName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint nRet = mysql_real_query(pConn, szSQL, strlen(szSQL));\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif (0 != nRet)\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn mysql_errno(pConn);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tMYSQL_RES* results = NULL;\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tMYSQL_ROW record;\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tresults = mysql_store_result(pConn);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\twhile ((record = mysql_fetch_row(results)));\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\t_Object_Data_Solt* pSolt = new _Object_Data_Solt();\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tC%s* p%s = new C%s();\n",
                             pObjectClass->m_strClassName.c_str(),
                             pObjectClass->m_strClassName.c_str(),
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tp%s->Init();\n",
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tp%s->Set_Data(\"%s\", record[%d]);\n",
                                     pObjectClass->m_strClassName.c_str(),
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                     k + 1);
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tp%s->Set_Data(\"%s\", atoi(record[%d]));\n",
                                     pObjectClass->m_strClassName.c_str(),
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                     k + 1);
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tp%s->Set_Data(\"%s\", record[0]);\n",
                             pObjectClass->m_strClassName.c_str(),
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tsprintf(pSolt->m_szUUID, \"%%s\", record[0]);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tpSolt->m_pObject = (IObject* )p%s;\n",
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tpSolt->m_nState = 2;\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tvecObjectList.push_back(pSolt);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tmysql_free_result(results);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn 0;\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int Save_%s_Data(MYSQL* pConn, vector<_Object_Data_Solt*>& vecObjectList);\n", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tchar szSQL[2048] = { '\\0' };\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tfor(int i = 0; i < (int)vecObjectList.size(); i++)\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tC%s* p%s = (C%s* )vecObjectList[i].m_pObject;\n",
                             pObjectClass->m_strClassName.c_str(),
                             pObjectClass->m_strClassName.c_str(),
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\t%s obj%s;\n",
                                 pObjectClass->m_vec_Object_Info[k].m_strType.c_str(),
                                 pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tp%s->Get_Data(\"%s\", obj%s);\n",
                                 pObjectClass->m_strClassName.c_str(),
                                 pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                 pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tsprintf(szSQL, \"insert into tb_%s(GUID, ", pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s, ", pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s)\\\n \t\t\t(",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%%s, ");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%%s, ");
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                        else
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%%d, ");
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                    }
                    else
                    {
                        if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%%s) ON DUPLICATE KEY UPDATE\\\n \t\t\t");
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                        else
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%%d)ON DUPLICATE KEY UPDATE\\\n \t\t\t");
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                    }
                }

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s = '%%s', ",
                                         pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                        else
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s = %%d, ",
                                         pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                    }
                    else
                    {
                        if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s = '%%s') \"\n \t\t\t",
                                         pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                        else
                        {
                            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "%s = '%%d')\"\n \t\t\t",
                                         pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                        }
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "vecObjectList[i]->m_szUUID, ");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "obs%s, ",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "obs%s, \n \t\t\t",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (k != (int)pObjectClass->m_vec_Object_Info.size() - 1)
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "obs%s, ",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "obs%s)\n\n",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str());
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint nRet = mysql_real_query(pConn, szSQL, strlen(szSQL));\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif (0 != nRet)\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn mysql_errno(pConn);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tmysql_commit(pConn);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn 0;\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                fclose(pFile);
            }
        }
    }

    return true;
}

bool CMysqlObject::Create_Mysql_Script(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };
    _ObjectClass* pObjectClass = NULL;

    //创建对应目录
    if (Create_Project_Path(MYSQL_OUTPUT_SCRIPT_PATH) == false)
    {
        return false;
    }

    for (int i = 0; i < (int)objMysqlDBList.size(); i++)
    {
        sprintf_safe(szHFileName, 200, "%s/%s_DB.sql", MYSQL_OUTPUT_SCRIPT_PATH, objMysqlDBList[i].m_strDBName.c_str());
        FILE* pFile = fopen(szHFileName, "w");

        for (int j = 0; j < (int)objMysqlDBList[i].m_vec_Xml_Mysql_Table.size(); j++)
        {
            pObjectClass = NULL;
            pObjectClass = Get_ObjectClass(objMysqlDBList[i].m_vec_Xml_Mysql_Table[j].m_nClassID, objObjectList);

            if (NULL != pObjectClass)
            {
                //创建mysql表创建语句
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "CREATE TABLE IF NOT EXISTS `tb_%s`(\n",
                             pObjectClass->m_strClassName.c_str());
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tGUID varchar(%d) primary key not null\n",
                             GUID_SIZE);
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

                for (int k = 0; k < (int)pObjectClass->m_vec_Object_Info.size(); k++)
                {
                    if (pObjectClass->m_vec_Object_Info[k].m_strAttribute == "STRING")
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t%s varchar(%d)\n",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                     Get_Object_Size(pObjectClass->m_vec_Object_Info[k].m_strType, obj_Base_Type_List_info));
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                    else
                    {
                        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t%s int(%d)\n",
                                     pObjectClass->m_vec_Object_Info[k].m_strName.c_str(),
                                     Get_Object_Size(pObjectClass->m_vec_Object_Info[k].m_strType, obj_Base_Type_List_info));
                        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    }
                }

                sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t);\n");
                fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            }
        }

        fclose(pFile);
    }

    return true;
}
