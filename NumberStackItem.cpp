#include "pch.h"

CNumberStackItem::CNumberStackItem()
{
	m_Value = 0;
}

CNumberStackItem::~CNumberStackItem()
{
}

bool CNumberStackItem::Create()
{
	return CStackItem::Create(CStackItem::ItemType::INTVALUE);
}
