#include "pch.h"

CObjFormatSymbolItem::CObjFormatSymbolItem()
{
	int i;

	m_DefinedAddress = 0;
	for (i = 0; i < MAX_SYMBOL_NAME_LEN; ++i)
		m_strName[i] = 0;
}

CObjFormatSymbolItem::~CObjFormatSymbolItem()
{
}

bool CObjFormatSymbolItem::Create(CSymbol* pSym)
{
	return 0;
}

void CObjFormatSymbolItem::Out(FILE* pOut)
{
}

void CObjFormatSymbolItem::Print(FILE* pO)
{
}
