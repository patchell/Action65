#include "pch.h"

bool CBin::Compare(const char* name, int aux)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
		rV = true;
    return rV;
}

void CBin::Print(FILE* pOut, const char* s)
{
	fprintf(pOut, "CBin::%s\n", m_pName);
}

//-----------------------------------------------
// Sub list methods
//-----------------------------------------------

void CBin::Add(CBin* pSym)
{
	if (GetHead())
	{
		GetTail()->SetNext(pSym);
		pSym->SetPrev(GetTail());
		SetTail(pSym);
	}
	else
	{
		SetTail(pSym);
		SetHead(pSym);
	}
}

CBin* CBin::Find(const char* pName, CBin::BinType Type)
{
	CBin* pBin = 0;
	bool loop = true;

	pBin = GetHead();
	while (pBin && loop)
	{
		if (pBin->Compare(pName, Type))
			loop = false;
		else
			pBin = pBin->GetNext();
	}
	return pBin;
}

void CBin::Delete(CBin* pSym)
{
	Unlink(pSym);
	delete pSym;
}

void CBin::Unlink(CBin* pSym)
{
	if (GetHead() == pSym)
	{
		SetHead(pSym->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
	}
	else if (GetTail() == pSym)
	{
		SetTail(pSym->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
	}
	else
	{
		pSym->GetNext()->SetPrev(pSym->GetPrev());
		pSym->GetPrev()->SetNext(pSym->GetNext());
	}
}
