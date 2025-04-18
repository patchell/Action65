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

void CTypeChain::MoveTypeChain(CTypeChain* pSrcTC)
{
	SetHead(pSrcTC->GetHead());
	SetTail(pSrcTC->GetTail());
	pSrcTC->SetHead(0);
	pSrcTC->SetHead(0);
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

bool CTypeChain::IsByte()
{
	//---------------------------------------
	// Determin if the type defined
	// references an object 8 bits or 16 bits
	//---------------------------------------
	bool rV = false;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && !rV)
	{
		if (pOT->Is(CObjTypeChain::Spec::BYTE))
			rV = true;
		else if (pOT->Is(CObjTypeChain::Spec::CHAR))
			rV = true;
		else if (pOT->Is(CObjTypeChain::Spec::BOOL))
			rV = true;
		else
			pOT = pOT->GetNext();
	}
	return rV;
}

bool CTypeChain::IsWord()
{
	bool rV = false;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && !rV)
	{
		if (pOT->Is(CObjTypeChain::Spec::CARD))
			rV = true;
		else if (pOT->Is(CObjTypeChain::Spec::INT))
			rV = true;
		else
			pOT = pOT->GetNext();
	}
	return rV;
}

bool CTypeChain::Is(CObjTypeChain::Spec SpecType)
{
	bool rV = false;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && !rV)
	{
		if (pOT->Is(SpecType))
			rV = true;
		else
			pOT = pOT->GetNext();
	}
	return rV;
}

CObjTypeChain::Spec CTypeChain::GetFundSpec()
{
	CObjTypeChain::Spec rSpec;
	bool Loop = true;
	CObjTypeChain* pOT;

	pOT = GetHead();
	while (pOT && Loop)
	{
		if (pOT->IsFundamentalType())
		{
			Loop = false;
			rSpec = pOT->GetSpec();
		}
		else
			pOT = pOT->GetNext();
	}

    return rSpec;
}

