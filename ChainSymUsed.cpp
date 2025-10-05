#include "pch.h"

CChainSymUsed::CChainSymUsed() : CChainItem(CChainItem::ChainItemType::SYMBOL_USED)
{
}

CChainSymUsed::~CChainSymUsed()
{
}

void CChainSymUsed::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::SYMBOL_USED)))
	{
		m_pInst = ((CChainSymUsed*)pI)->m_pInst;
	}
}

bool CChainSymUsed::Compare(const char* pName)
{
	bool rV = false;
	char* pLabelName = 0;

	if (m_pInst)
	{
		pLabelName = m_pInst->GetLabel()->GetName();
		if (pName)
		{
			if (pLabelName)
			{
				if (strcmp(pLabelName, pName) == 0)
					rV = true;
			}
		}
	}
	return rV;
}