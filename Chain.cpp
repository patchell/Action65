#include "pch.h"

CChain::CChain()
{
	m_pHead = 0;
	m_pTail = 0;
	m_ChainType = ChainType::NONE;
}

CChain::CChain(ChainType T)
{
	m_pHead = 0;
	m_pTail = 0;
	m_ChainType = T;
}

CChain::~CChain()
{
}

bool CChain::Create()
{
	return true;
}

void CChain::Copy(CChain* pC)
{
	CChainItem::ChainItemType ItemType;

	if (pC)
	{
		CChainItem* pItem = pC->GetHead();
		while (pItem)
		{
			CChainItem* pNewItem = 0;
			ItemType = pItem->GetItemType();
			switch (ItemType)
			{
			case CChainItem::ChainItemType::LOCAL_SYMBOL:
				pNewItem = new CChainLocalItem();
				break;
			case CChainItem::ChainItemType::INSTRUCTION:
				pNewItem = new CChainInstruction();
				break;
			case CChainItem::ChainItemType::VALUE:
				pNewItem = new CChainValueItem();
				break;
			case CChainItem::ChainItemType::BIN:
				pNewItem = new CChainBinItem();
				break;
			case CChainItem::ChainItemType::PARAMETER:
				pNewItem = new CChainParameterItem();
				break;
			case CChainItem::ChainItemType::TYPE:
				pNewItem = new CChainTypeSpecItem();
				break;
			case CChainItem::ChainItemType::SYMBOL_USED:
				pNewItem = new CChainSymUsedItem();
				break;
			default:
				break;
			}
			if (pNewItem)
			{
				if (pNewItem->Create())
				{
					pNewItem->Copy(pItem);
					AddToTail(pNewItem);
				}
				else
				{
					delete pNewItem;
					pNewItem = 0;
				}
			}
			pItem = pItem->GetNext();
		}
	}
}

void CChain::AddToTail(CChainItem* pItem)
{
	if (GetHead())
	{
		GetTail()->SetNext(pItem);
		pItem->SetPrev(GetTail());
		SetTail(pItem);
	}
	else
	{
		SetTail(pItem);
		SetHead(pItem);
	}
}

void CChain::AddToHead(CChainItem* pItem)
{
	if (GetHead())
	{
		GetHead()->SetPrev(pItem);
		pItem->SetNext(GetTail());
		SetHead(pItem);
	}
	else
	{
		SetTail(pItem);
		SetHead(pItem);
	}
}

void CChain::Unlink(CChainItem* pItem)
{
	if (GetHead() == pItem)
	{
		SetHead(pItem->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
	}
	else if (GetTail() == pItem)
	{
		SetTail(pItem->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
	}
	else
	{
		pItem->GetNext()->SetPrev(pItem->GetPrev());
		pItem->GetPrev()->SetNext(pItem->GetNext());
	}
}

CChainItem* CChain::FindItem(const char* pName)
{
	CChainItem* pChainItem = 0;
	bool Loop = true;

	pChainItem = GetHead();
	while (pChainItem && Loop)
	{
		if (pChainItem->Compare(pName))
			Loop = false;
		else
			pChainItem = pChainItem->GetNext();
	}
	return pChainItem;
}

CChain::ChainType CChain::LookupType(const char* pName)
{
	ChainType rV = ChainType::NONE;

    for (int i = 0; TypeItemLUT[i].m_pName && !bool(rV); ++i)
    {
		if (strcmp(TypeItemLUT[i].m_pName, pName) == 0)
            rV = TypeItemLUT[i].m_Type;
    }
    return rV;
}

const char* CChain::FindTypeName(ChainType T) const
{
	const char* rV = nullptr;

    for (int i = 0;TypeItemLUT[i].m_pName && !rV; ++i)
    {
        if (TypeItemLUT[i].m_Type == T)
            rV = TypeItemLUT[i].m_pName;
    }
    return rV;
}

int CChain::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	CChainItem* pItem = GetHead();
	if (s)
	{
		ls += sprintf_s(&pSO[ls], l - ls, "Head:%s", s);
	}
	while (pItem)
	{
		int size = l - ls;
		ls += pItem->Print(&pSO[ls], size, Indent+2, "");
		pItem = pItem->GetNext();
	}
	return ls;
}