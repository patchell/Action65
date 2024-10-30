#include "pch.h"


CBucket::~CBucket()
{
	CBin* pBin;

	pBin = m_pHead;

	while (pBin)
	{
		pBin->Print(stdout);
		m_pHead = pBin->GetNext();
		delete pBin;
		pBin = m_pHead;
	}
}

void CBucket::Add(CBin* pSym)
{
	if (m_pHead)
	{
		m_pTail->SetNext(pSym);
		pSym->SetPrev(m_pTail);
		m_pTail = pSym;
	}
	else
	{
		m_pTail = pSym;
		m_pHead = pSym;
	}
}

CBin* CBucket::Find(const char* pName, int scope)
{
	CBin* pBin = 0;
	bool loop = true;

	pBin = GetHead();
	while (pBin && loop)
	{
		if (pBin->Compare(pName, scope))
			loop = false;
		else
			pBin = pBin->GetNext();
	}
	return pBin;
}

void CBucket::Delete(CBin* pSym)
{
	Unlink(pSym);
	delete pSym;
}

void CBucket::Unlink(CBin* pSym)
{
	if (m_pHead == pSym)
	{
		m_pHead = pSym->GetNext();
		m_pHead->SetPrev(0);
	}
	else if (m_pTail == pSym)
	{
		m_pTail = pSym->GetPrev();
		m_pTail->SetNext(0);
	}
	else
	{
		pSym->GetNext()->SetPrev(pSym->GetPrev());
		pSym->GetPrev()->SetNext(pSym->GetNext());
	}
}

