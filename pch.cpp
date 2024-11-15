#include "pch.h"


Exception ThrownException;

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

const char* Exception::ExcepTypeToString::FindString(ExceptionType Type)
{
	const char* pString = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && (ExceptionTypesLUT[i].m_Name != 0 ); ++i)
	{
		if (ExceptionTypesLUT[i].m_Type == Type)
		{
			pString = ExceptionTypesLUT[i].m_Name;
			Loop = false;
		}
	}
    return pString;
}

const char* Exception::ExceptionSubTypeStrings::FindSubTypeString(ExceptionSubType Type)
{
	const char* pString = 0;
	int i;
	bool Loop = true;

	for (i = 0; Loop && (XcepSubTypesLUT[i].m_Name != 0); ++i)
	{
		if (XcepSubTypesLUT[i].m_Type == Type)
		{
			pString = XcepSubTypesLUT[i].m_Name;
			Loop = false;
		}
	}
	return pString;
}
