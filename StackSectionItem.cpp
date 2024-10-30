#include "pch.h"

CStackSectionItem::CStackSectionItem()
{
}

CStackSectionItem::~CStackSectionItem()
{
}

bool CStackSectionItem::Create(CSection* pSect)
{
	bool rV = true;
	CStackItem::Create(CStackItem::ItemType::SECTION);
	SetSection(pSect);
	return true;
}
