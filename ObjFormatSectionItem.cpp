#include "pch.h"

CObjFormatSectionItem::CObjFormatSectionItem()
{
	m_Type = FormatType::NONE;
	m_pNext = 0;
	m_pPrev = 0;
}

CObjFormatSectionItem::~CObjFormatSectionItem()
{
}

bool CObjFormatSectionItem::Create(FormatType Type)
{
	bool rV = true;
	m_Type = Type;
	return rV;
}

void CObjFormatSectionItem::Out(FILE* pOut)
{
}

void CObjFormatSectionItem::Print(FILE* pO, int Indent)
{
}

const char* CObjFormatSectionItem::LookupFormatSectionType(FormatType Type)
{
	const char* pStr = 0;;
	int i;
	bool Loop = true;

	for (i = 0; FormatStringLUT[i].m_pLutString && Loop; ++i)
	{
		if (Type == FormatStringLUT[i].m_LutType)
		{
			pStr = FormatStringLUT[i].m_pLutString;
			Loop = false;
		}
	}
	return pStr;
}

CObjFormatSectionItem::FormatType CObjFormatSectionItem::LookupFormatSectionType(const char* pName)
{	
	FormatType Type = FormatType::NONE;
	int i;
	bool Loop = true;

	for (i = 0; FormatStringLUT[i].m_pLutString && Loop; ++i)
	{
		if (strcmp(FormatStringLUT[i].m_pLutString, pName) == 0)
		{
			Type = FormatStringLUT[i].m_LutType;
			Loop = false;
		}
	}
	return Type;
}
