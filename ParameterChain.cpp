#include "pch.h"

CParameterChain::CParameterChain()
{
	m_pHead = 0;
	m_pTail = 0;
}

CParameterChain::~CParameterChain()
{
	CBin* pSym = 0;
	CBin* pNxtBin;

	pSym = GetTail();
	while (pSym)
	{
		pNxtBin = pSym->GetPrev();
		delete pSym;
		pSym = pNxtBin;
	}
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

CBin* CParameterChain::Find(const char* pName)
{
	CBin* pSym = 0;
	bool Loop = true;

	pSym = GetHead();
	while (pSym && Loop)
	{
		if (strcmp(pName, pSym->GetName()))
			Loop = false;
		else
			pSym = pSym->GetNext();
	}
    return pSym;
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
