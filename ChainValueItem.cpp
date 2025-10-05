#include "pch.h"

CChainValueItem::CChainValueItem() :CChainItem(ChainItemType::VALUE)
{
	m_pValue = 0;
	m_Location = 0;
}

CChainValueItem::~CChainValueItem()
{
}

void CChainValueItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::VALUE)))
	{
		m_pValue = ((CChainValueItem*)pI)->m_pValue;
		m_Location = ((CChainValueItem*)pI)->m_Location;
	}
}

bool CChainValueItem::Compare(const char* pName)
{
	return false;
}
