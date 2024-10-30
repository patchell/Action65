#include "pch.h"

CAccessModeStackItem::CAccessModeStackItem()
{
	m_AccessMode = CSection::Mode::MODE_READ_WRITE;
}

CAccessModeStackItem::~CAccessModeStackItem()
{
}

bool CAccessModeStackItem::Create()
{
	return CStackItem::Create(CStackItem::ItemType::ACCESS_MODE);
}
