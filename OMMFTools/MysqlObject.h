#ifndef _MYSQL_OBJECT
#define _MYSQL_OBJECT

#include "Common.h"
#include "TemplateObjectList.h"

class CMysqlObject
{
public:
    CMysqlObject();
    ~CMysqlObject();

    //生成mysql相关文件
    bool WriteMysqlCode(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);

private:
    //根据ClassID得到对象信息
    _ObjectClass* Get_ObjectClass(int nClassID, vec_ObjectClass& objObjectList);
    //根据指定的变量类型获得当前变量的长度
    int Get_Object_Size(string strName, _Base_Type_List_info obj_Base_Type_List_info);
    //创建Mysql的头文件
    bool Create_Mysql_Code_H(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
    //创建Mysql读写的cpp文件
    bool Create_Mysql_Code_Cpp(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
    //创建Mysql的脚本
    bool Create_Mysql_Script(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
};
#endif
