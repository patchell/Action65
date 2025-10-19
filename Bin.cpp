#include "pch.h"

int CBin::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;

	char* pIndentString = new char[512];
	Act()->IndentString(pIndentString, 512, Indent + 2, ' ');
	if (!s) s = "";		

	ls += sprintf_s(&pSO[ls], size, "%sCBin::%s\n", 
		pIndentString, 
		GetName());


	delete[] pIndentString;
	return ls;
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
