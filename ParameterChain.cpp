#include "pch.h"

CParameterChain::CParameterChain()
{
	m_pHead = 0;
	m_pTail = 0;
}

CParameterChain::~CParameterChain()
{
}

bool CParameterChain::Create()
{
	return false;
}

void CParameterChain::AddToTail(CBin* pB)
{
	if (GetHead())
	{
		GetTail()->SetNext(pB);
		pB->SetPrev(GetTail());
		SetTail(pB);
	}
	else
	{
		SetTail(pB);
		SetHead(pB);
	}
}

void CParameterChain::AddToHead(CBin* pB)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pB);
		pB->SetNext(GetTail());
		SetHead(pB);
	}
	else
	{
		SetTail(pB);
		SetHead(pB);
	}
}

int CParameterChain::Print(char* pS, int l)
{
	CSymbol* pSym;
	int ls = 0;

	pSym = (CSymbol*)GetHead();
	while (pSym)
	{
		ls += pSym->Print(&pS[ls], l - ls, "\t");
		pSym = (CSymbol*)pSym->GetNext();
	}
	return ls;
}
