#include "pch.h"

CChainValueItem::CChainValueItem()
{
	m_pValue = 0;
	m_Location = 0;
}

CChainValueItem::~CChainValueItem()
{
}

bool CChainValueItem::Compare(const char* pName)
{
	return false;
}
