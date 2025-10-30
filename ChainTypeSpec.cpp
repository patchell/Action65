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
		if (pItem->Is(CChainItem::ChainItemType::TYPESPEC))
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

	if (pItem && pItem->Is(CChainItem::ChainItemType::TYPESPEC))
	{
		CChainTypeSpecItem* pTO = (CChainTypeSpecItem*)pItem;
		rV = pTO->IsFundamentalType();
	}
	return rV;
}

CChainTypeSpecItem::Spec CChainTypeSpec::GetScope()
{
	CChainTypeSpecItem* pItem = (CChainTypeSpecItem*)GetHead();
	CChainTypeSpecItem::Spec rV = CChainTypeSpecItem::Spec::NONE;

	if (pItem)
	{
		if (pItem->IsScopeType())
			rV = pItem->GetSpec();
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

int CChainTypeSpec::SizeOf()
{
	int Size = 0;

	CChainTypeSpecItem* pItem = (CChainTypeSpecItem*)GetHead();

	while (pItem)
	{
		switch(pItem->GetSpec())
		{
		case CChainTypeSpecItem::Spec::BYTE:
		case CChainTypeSpecItem::Spec::CHAR:
		case CChainTypeSpecItem::Spec::BOOL:
			if (Size < 1 )
				Size = 1;
			break;
		case CChainTypeSpecItem::Spec::INT:
		case CChainTypeSpecItem::Spec::CARD:
		case CChainTypeSpecItem::Spec::POINTER:
			if (Size < 2)
				Size = 2;
			break;
		case CChainTypeSpecItem::Spec::ARRAY:
			// TO DO: Handle arrays properly
			break;
		case CChainTypeSpecItem::Spec::CONST:
			break;
		}
		pItem = (CChainTypeSpecItem*)pItem->GetNext();
	}
	return Size;
}
