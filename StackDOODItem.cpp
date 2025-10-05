#include "pch.h"

CStackDOODItem::CStackDOODItem() :CStackItem(CStackItem::ItemType::DOOD)
{
	m_pLabelValue = 0;
}

CStackDOODItem::~CStackDOODItem()
{
}

bool CStackDOODItem::Create(CValue* pLabelVal)
{
	m_pLabelValue = pLabelVal;
	return false;
}
