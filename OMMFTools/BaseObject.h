//create BaseObject.h

#ifndef _BASEOBJECT_H
#define _BASEOBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class IObject
{
public:
	virtual ~IObject() {};
	virtual bool Get_Stream(char* pData, int& nLen) = 0;
	virtual bool Set_Stream(char* pData, int& nLen) = 0;
};

#endif
