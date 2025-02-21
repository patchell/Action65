#include "pch.h"

CTypeChain::~CTypeChain()
{
	CObjTypeChain* pH, * pN;

	pH = GetHead();
	while (pH)
	{
		pN = pH->GetNext();
		delete pH;
		pH = pN;
	}
}

bool CTypeChain::Create()
{
	return true;
}

void CTypeChain::AddToTail(CObjTypeChain* pOTC)
{
	if (GetTail())
	{
		GetTail()->SetNext(pOTC);
		pOTC->SetPrev(GetTail());
		SetTail(pOTC);
	}
	else
	{
		SetTail(pOTC);
		SetHead(pOTC);
	}
}

void CTypeChain::AddToHead(CObjTypeChain* pOTC)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pOTC);
		pOTC->SetNext(GetTail());
		SetHead(pOTC);
	}
	else
	{
		SetTail(pOTC);
		SetHead(pOTC);
	}
}

void CTypeChain::Delete(CObjTypeChain* pOTC)
{
	Unlink(pOTC);
	delete pOTC;
}

void CTypeChain::Unlink(CObjTypeChain* pOTC)
{
	if (GetHead() == pOTC)
	{
		SetHead(pOTC->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
	}
	else if (GetTail() == pOTC)
	{
		SetTail(pOTC->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
	}
	else
	{
		pOTC->GetNext()->SetPrev(pOTC->GetPrev());
		pOTC->GetPrev()->SetNext(pOTC->GetNext());
	}
}

void CTypeChain::CopyTypeChain(CTypeChain* pSrcTC)
{
	CObjTypeChain* pOCT = 0;
	CObjTypeChain* pNewOCT = 0;

	if (pSrcTC)
	{
		pOCT = pSrcTC->GetHead();
		while (pOCT)
		{
			pNewOCT = new CObjTypeChain;
			pNewOCT->Create();
			pNewOCT->SetSpec(pOCT->GetSpec());
			AddToTail(pNewOCT);
			pOCT = pOCT->GetNext();
		}
	}
}

int CTypeChain::Print(char* pSO, int l)
{
	int ls = 0;
	CObjTypeChain* pOTC = GetHead();

	while (pOTC) //fucked up here
	{
		ls += pOTC->Print(&pSO[ls], l - ls);
		pOTC = pOTC->GetNext();
	}
	return ls;
}

bool CTypeChain::IsGlobal()
{
	bool rV = false;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && !rV)
	{
		if (pOT->IsGlobal())
			rV = true;
		else
			pOT = pOT->GetNext();
	}
    return rV;
}

bool CTypeChain::IsLocal()
{
	bool rV = false;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && !rV)
	{
		if (pOT->IsLocal())
			rV = true;
		else
			pOT = pOT->GetNext();
	}
	return rV;
}

