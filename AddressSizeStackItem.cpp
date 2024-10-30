#include "pch.h"

CAddressSizeStackItem::CAddressSizeStackItem()
{
	m_OpRandAdrSize = CSection::AddressSize::ADDRESSSIZE_WORD; //default
}

CAddressSizeStackItem::~CAddressSizeStackItem()
{
}

bool CAddressSizeStackItem::Create()
{
	return CStackItem::Create(CStackItem::ItemType::ADDRESSSIZE);
}
