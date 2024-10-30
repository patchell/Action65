#include "pch.h"


Exception ExceptionThrown;

const char* Exception::GetExceptionTypeString(ExceptionType xType)
{
	const char* pName = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && ExceptionTypesLUT[i].m_Name; ++i)
	{
		if (ExceptionTypesLUT[i].m_Type == xType)
		{
			pName = ExceptionTypesLUT[i].m_Name;
			Loop = false;
		}
	}
	return pName;
}
