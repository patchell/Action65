#include "pch.h"

CChainParameter::CChainParameter() :CChain(CChain::ChainType::CHAINPARAMETERS)
{
}

CChainParameter::~CChainParameter()
{
}

bool CChainParameter::Create()
{
	return CChain::Create();
}

CSymbol* CChainParameter::FindParamSymbol(const char* pName)
{
	CChainParameterItem* pItem = (CChainParameterItem*)GetHead();
	bool bFound = false;
	CSymbol* pSym = 0;
	while (pItem && !bFound)
	{
		if (pItem->Is(pName))
		{
			bFound = true;
			pSym = pItem->GetSymbol();
		}
		pItem = (CChainParameterItem*)pItem->GetNext();
	}
	return pSym;
}


CValue* CChainParameter::FindParamValue(const char* pName)
{
	CChainParameterItem* pItem = (CChainParameterItem*)GetHead();
	bool bFound = false;
	CValue* pValue = nullptr;
	while (pItem && !bFound)
	{
		if (pItem->Is(pName))
		{
			bFound = true;
			pValue = pItem->GetValue();
		}
		pItem = (CChainParameterItem*)pItem->GetNext();
	}
	return pValue;
}
