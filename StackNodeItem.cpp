#include "pch.h"

CStackNodeItem::CStackNodeItem()
{
	m_pAstNode = 0;
}

CStackNodeItem::~CStackNodeItem()
{
}

bool CStackNodeItem::Create(CAstNode* pNode)
{
	bool rV = true;
	CStackItem::Create(CStackItem::ItemType::NODE);
	SetNode(pNode);
	return rV;
}
