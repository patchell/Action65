#include "pch.h"

CChainLocals::CChainLocals() :CChain(CChain::ChainType::CHAINLOCALS)
{
}

CChainLocals::~CChainLocals()
{
}

bool CChainLocals::Create()
{
	return CChain::Create();
}

CSymbol* CChainLocals::FindLocalSymbol(const char* pName)
{
	CChainLocalItem* pItem = (CChainLocalItem *) GetHead();
	bool bFound = false;
	CSymbol* pSym = 0;

	while (pItem && !bFound)
	{
		if (pItem->Is(pName))
		{
			bFound = true;
			pSym = pItem->GetSymbol();
		}
		pItem = (CChainLocalItem*)pItem->GetNext();
	}
	return pSym;
}

CValue* CChainLocals::FindLocalValue(const char* pName)
{
	CValue* pValue = nullptr;
	CChainLocalItem* pItem = (CChainLocalItem*)GetHead();
	bool bFound = false;

	while (pItem && !bFound)
	{
		if (pItem->Is(pName))
		{
			bFound = true;
			pValue = pItem->GetValue();
		}
		pItem = (CChainLocalItem*)pItem->GetNext();
	}
	return pValue;
}

