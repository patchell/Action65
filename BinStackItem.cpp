#include "pch.h"

CBinStackItem::CBinStackItem()
{
	m_pBin = 0;
}

CBinStackItem::~CBinStackItem()
{
}

bool CBinStackItem::Create()
{
	return CStackItem::Create(CStackItem::ItemType::BIN);
}
