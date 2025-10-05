#include "pch.h"

CChainBinItem::CChainBinItem() :CChainItem(ChainItemType::BIN)
{
	m_pBin = 0;
}

CChainBinItem::~CChainBinItem()
{
}

void CChainBinItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::BIN)))
	{
		m_pBin = ((CChainBinItem*)pI)->m_pBin;
	}
}

bool CChainBinItem::Compare(const char* pName)
{
	bool rV = false;

	if (strcmp(pName,GetSymbol()->GetName()) == 0)
		rV = true;
	return rV;
}
