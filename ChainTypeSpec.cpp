#include "pch.h"

CChainTypeSpec::CChainTypeSpec() : CChain(CChain::ChainType::CHAINTYPESPEC)
{
}

bool CChainTypeSpec::Create()
{
	return CChain::Create();
}

bool CChainTypeSpec::Is(CChainTypeSpecItem::Spec Type)
{
	bool rV = false;
	CChainItem* pItem = GetHead();

	while (pItem && !rV)
	{
		if (pItem->Is(CChainItem::ChainItemType::TYPE))
		{
			CChainTypeSpecItem* pTO = (CChainTypeSpecItem*)pItem;
			if (pTO->Is(Type))
			{
				rV = true;
			}
		}
		pItem = pItem->GetNext();
	}
	return rV;
}

bool CChainTypeSpec::IsFundamentalType()
{
	bool rV = false;
	CChainItem* pItem = GetHead();

	if (pItem && pItem->Is(CChainItem::ChainItemType::TYPE))
	{
		CChainTypeSpecItem* pTO = (CChainTypeSpecItem*)pItem;
		rV = pTO->IsFundamentalType();
	}
	return rV;
}

CChainTypeSpecItem::Spec CChainTypeSpec::GetFundSpec()
{
	CChainTypeSpecItem* pItem = (CChainTypeSpecItem*)GetHead();
	CChainTypeSpecItem::Spec rV = CChainTypeSpecItem::Spec::NONE;

	if (pItem)
	{
		if (pItem->IsFundamentalType())
			rV = pItem->GetSpec();
	}
	return rV;
}
