#ifndef _READFILE_H
#define _READFILE_H

#include "Common.h"

class CReadObject
{
public:
    CReadObject();
    ~CReadObject();

    bool WriteClass(int i, vec_ObjectClass objObjectClassList);
};

#endif
