#include "pch.h"

CChainSymUsedItem::CChainSymUsedItem() :CChainItem(CChainItem::ChainItemType::SYMBOL_USED)
{
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
	ls = m_pInstruction->Print(&pSO[ls], l - ls, Indent + 2, s);
	return ls;
}

