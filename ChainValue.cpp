#include "pch.h"

CChainValue::CChainValue() :CChain(CChain::ChainType::CHAINVALUES)
{
}

bool CChainValue::Create()
{
	return CChain::Create();
}

void CChainValue::Copy(CChain* pC)
{
	CChain::Copy(pC);
}	

int CChainValue::Print(char* pSO, int l, int Indent, const char* s)
{
	return CChain::Print(pSO, l, Indent, s);
}	
