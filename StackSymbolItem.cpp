#include "pch.h"

CStackSymbolItem::CStackSymbolItem()
{
	m_pSymbol = 0;
}

CStackSymbolItem::~CStackSymbolItem()
{
}

bool CStackSymbolItem::Create(CSymbol* pSym)
{
	bool rV = true;

	CStackItem::Create(CStackItem::ItemType::SYMBOL);
	SetSymbol(pSym);
	return rV;
}
