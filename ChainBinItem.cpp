#include "pch.h"

CChainBinItem::CChainBinItem()
{
}

CChainBinItem::~CChainBinItem()
{
}

bool CChainBinItem::Compare(const char* pName)
{
	bool rV = false;

	if (strcmp(pName,GetSymbol()->GetName()) == 0)
		rV = true;
	return rV;
}
