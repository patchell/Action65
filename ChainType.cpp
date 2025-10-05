#include "pch.h"

CChainType::CChainType() : CChain(CChain::ChainType::CHAINTYPE)
{
}

bool CChainType::Create()
{
	return CChain::Create();
}

bool CChainType::Is(CChainTypeObject::Spec Type)
{
	bool rV = false;
	CChainItem* pItem = GetHead();

	while (pItem && !rV)
	{
		if (pItem->Is(CChainItem::ChainItemType::TYPE))
		{
			CChainTypeObject* pTO = (CChainTypeObject*)pItem;
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
		CChainTypeObject* pTO = (CChainTypeObject*)pItem;
		rV = pTO->IsFundamentalType();
	}
	return rV;
}

CChainTypeObject::Spec CChainType::GetFundSpec()
{
	CChainTypeObject* pItem = (CChainTypeObject*)GetHead();
	CChainTypeObject::Spec rV = CChainTypeObject::Spec::NONE;

	if (pItem)
	{
		if (pItem->IsFundamentalType())
			rV = pItem->GetSpec();
	}
	return rV;
}
