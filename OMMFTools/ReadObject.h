#ifndef _READFILE_H
#define _READFILE_H

#include "Common.h"

class CReadObject
{
public:
    CReadObject();
    ~CReadObject();

    //根据XML生成创建函数代码
    bool WriteFunction(_Function_Info& obj_Function_Info);
    //根据XML生成创建消息代码
    bool WriteMessage(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);
    //生成某一个对象的实现类代码
    bool WriteClass(int i, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //生成ObjectManager类
    bool WriteListManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //生成测试类代码
    bool WriteTestManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);

private:
    //判断一个变量是不是基础类型
    bool Check_Type_In_Class(string strBaseTypeName, vec_Object_Info obj_vec_Object_Info);
    //生成类代码h文件
    bool Create_Object_H(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //生成类代码cpp文件
    bool Create_Object_Cpp(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //生成消息代码文件
    bool Create_Message_H(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);
    //生成函数代码h文件
    bool Create_Function_H(_Function_Info& obj_Function_Info);
    //生成函数代码cpp文件
    bool Create_Function_Cpp(_Function_Info& obj_Function_Info);
    //生成ObjectManager管理h文件
    bool Create_List_Manager_H(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //生成ObjectManager管理cpp文件
    bool Create_List_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //生成测试Test工程代码
    bool Create_Test_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //生成Test测试的makefile代码
    bool Create_Test_Make(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //生成Test测试的Make.Define代码
    bool Create_Test_Make_Define(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
};

#endif
