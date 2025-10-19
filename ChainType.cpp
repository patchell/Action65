#include "pch.h"

CChainType::CChainType() : CChain(CChain::ChainType::CHAINTYPE)
{
}

bool CChainType::Create()
{
	return CChain::Create();
}

bool CChainType::Is(CChainTypeItem::Spec Type)
{
	bool rV = false;
	CChainItem* pItem = GetHead();

	while (pItem && !rV)
	{
		if (pItem->Is(CChainItem::ChainItemType::TYPE))
		{
			CChainTypeItem* pTO = (CChainTypeItem*)pItem;
			if (pTO->Is(Type))
			{
				rV = true;
			}
		}
		pItem = pItem->GetNext();
	}
	return rV;
}

bool CChainType::IsFundamentalType()
{
	bool rV = false;
	CChainItem* pItem = GetHead();

	if (pItem && pItem->Is(CChainItem::ChainItemType::TYPE))
	{
		CChainTypeItem* pTO = (CChainTypeItem*)pItem;
		rV = pTO->IsFundamentalType();
	}
	return rV;
}

CChainTypeItem::Spec CChainType::GetFundSpec()
{
	CChainTypeItem* pItem = (CChainTypeItem*)GetHead();
	CChainTypeItem::Spec rV = CChainTypeItem::Spec::NONE;

	if (pItem)
	{
		if (pItem->IsFundamentalType())
			rV = pItem->GetSpec();
	}
	return rV;
}
