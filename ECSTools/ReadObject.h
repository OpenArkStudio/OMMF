#ifndef _READFILE_H
#define _READFILE_H

#include "Common.h"

class CReadObject
{
public:
    CReadObject();
    ~CReadObject();

    bool WriteClass(int i, vec_ObjectClass objObjectClassList);

private:
    bool Create_Object_H(int i, vec_ObjectClass objObjectClassList);
    bool Check_Object_exist(string strClassName, vec_ObjectClass objObjectClassList);
};

#endif
