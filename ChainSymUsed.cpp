#include "pch.h"

CChainSymUsed::CChainSymUsed() : CChain(CChain::ChainType::CHAINSYMBOLSUSED)
{
}

CChainSymUsed::~CChainSymUsed()
{
}

void CChainSymUsed::Copy(CChainItem* pI)
{
}

int CChainSymUsed::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;

	CChainItem* pItem = GetHead();
	while(pItem)
	{
		ls += pItem->Print(&pSO[ls], l - ls, Indent, s);
		pItem = pItem->GetNext();
	}
	return ls;
}