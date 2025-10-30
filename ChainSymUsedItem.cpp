#include "pch.h"

CChainSymUsedItem::CChainSymUsedItem() :CChainItem(CChainItem::ChainItemType::SYMBOL_USED)
{
	m_pUsedHere = 0;
}

CChainSymUsedItem::~CChainSymUsedItem()
{
}

bool CChainSymUsedItem::Create()
{
	return true;
}

void CChainSymUsedItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::SYMBOL_USED)))
	{
		// Implementation of Copy method
	}
}

int CChainSymUsedItem::Print(char* pSO, int l, int Indent, const char* s)
{
	// Implementation of Print function
	int ls = 0;
	char* pOprd = new char[256];
	memset(pOprd, 0, 256);

	ls = m_pUsedHere->Print(&pSO[ls], l - ls, Indent + 2, s);
	delete [] pOprd;
	return ls;
}

