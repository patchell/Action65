#include "pch.h"

CRegisterStackItem::CRegisterStackItem()
{
	m_RegType = RegType::NONE;
}

CRegisterStackItem::~CRegisterStackItem()
{
}

bool CRegisterStackItem::Create()
{
	CStackItem::Create(CStackItem::ItemType::REGTYPE);
	return true;
}
