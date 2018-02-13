#ifndef _READFILE_H
#define _READFILE_H

#include "Common.h"

class CReadObject
{
public:
    CReadObject();
    ~CReadObject();

    bool WriteFunction(_Function_Info& obj_Function_Info);
    bool WriteMessage(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);
    bool WriteClass(int i, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    bool WriteListManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    bool WriteTestManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);

private:
    bool Check_Type_In_Class(string strBaseTypeName, vec_Object_Info obj_vec_Object_Info);
    bool Create_Object_H(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    bool Create_Object_Cpp(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);

    bool Create_Message_H(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);

    bool Create_Function_H(_Function_Info& obj_Function_Info);
    bool Create_Function_Cpp(_Function_Info& obj_Function_Info);

    bool Create_List_Manager_H(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    bool Create_List_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);

    bool Create_Test_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    bool Create_Test_Make(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    bool Create_Test_Make_Define(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
};

#endif
