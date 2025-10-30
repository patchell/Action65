#include "pch.h"

CChainItemItem::CChainItemItem() :CChainItem(CChainItem::ChainItemType::SUBITEM)
{
	m_pItem = 0;
}

CChainItemItem::~CChainItemItem()
{
}

void CChainItemItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::SUBITEM)))
	{
		// Implementation of Copy method
	}
}

bool CChainItemItem::Compare(const char* pName)
{
	bool rV = false;
	// Implementation of Compare method
	return rV;
}

int CChainItemItem::Print(char* pSO, int l, int Indent, const char* s)
{
	// Implementation of Print function
	int ls = 0;

	ls = m_pItem->Print(&pSO[ls], l - ls, Indent + 2, s);
	return ls;
}

void CChainItemItem::Emit(CSection* pSec)
{
	// Implementation of Emit method
}	

void CChainItemItem::EmitListing(CSection* pSec)
{
	// Implementation of EmitListing method
}