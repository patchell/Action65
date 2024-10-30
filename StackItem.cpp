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

const char* CStackItem::LookupItemName(ItemType Type)
{
	int i;
	const char* s = NULL;
	bool Loop = true;

	for (i = 0; TypeString[i].m_pName && Loop; ++i)
	{
		if (Type == TypeString[i].m_Type)
		{
			Loop = false;
			s = TypeString[i].m_pName;
		}
	}
    return s;
}

const char* CStackItem::GetStackItemName()
{
	return LookupItemName(GetStackItemType());
}
