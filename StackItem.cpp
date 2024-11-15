#include "pch.h"

CStackItem::CStackItem()
{
	m_pNext = 0;
	m_Type = ItemType::NONE;
}

CStackItem::~CStackItem()
{
}

bool CStackItem::Create(ItemType Type)
{
	bool rV = true;

	m_Type = Type;
	return rV;
}

bool CStackItem::Verify(CStackItem::ItemType Type)
{
	bool rV = false;

	if (Type == GetStackItemType())
		rV = true;
	return rV;
}

const char* CStackItem::LookupItemName(CStackItem::ItemType Type)
{
	return CStackItem::ItemTypeLUT::FindItemName( Type);
}


const char* CStackItem::ItemTypeLUT::FindItemName(CStackItem::ItemType Type)
{
	const char* pName = 0;
	int i;
	bool Loop = true;

	for (i = 0;Loop && TypeString[i].m_pName; ++i)
	{
		if (Type == TypeString[i].m_Type)
		{
			pName = TypeString[i].m_pName;
			Loop = false;
		}
	}
    return pName;
}
