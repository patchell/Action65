#include "pch.h"

CChain::CChain()
{
}

CChain::~CChain()
{
}

void CChain::AddToTail(CChainItem* pItem)
{
	if (GetHead())
	{
		GetTail()->SetNext(pItem);
		pItem->SetPrev(GetTail());
		SetTail(pItem);
	}
	else
	{
		SetTail(pItem);
		SetHead(pItem);
	}
}

void CChain::AddToHead(CChainItem* pItem)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pItem);
		pItem->SetNext(GetTail());
		SetHead(pItem);
	}
	else
	{
		SetTail(pItem);
		SetHead(pItem);
	}
}

void CChain::Unlink(CChainItem* pItem)
{
	if (GetHead() == pItem)
	{
		SetHead(pItem->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
	}
	else if (GetTail() == pItem)
	{
		SetTail(pItem->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
	}
	else
	{
		pItem->GetNext()->SetPrev(pItem->GetPrev());
		pItem->GetPrev()->SetNext(pItem->GetNext());
	}
}

CChainItem* CChain::FindItem(const char* pName)
{
	CChainItem* pChainItem = 0;
	bool Loop = true;

	pChainItem = GetHead();
	while (pChainItem && Loop)
	{
		if (pChainItem->Compare(pName))
			Loop = false;
		else
			pChainItem = pChainItem->GetNext();
	}
	return pChainItem;
}
