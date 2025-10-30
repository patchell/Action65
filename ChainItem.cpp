#include "pch.h"

CChainItem::CChainItem()
{
	m_ItemType = ChainItemType::NONE;
	m_pNext = 0;
	m_pPrev = 0;
	m_ItemDataSize = 0;
}

CChainItem::~CChainItem()
{
}

//int CChainItem::Print(char* pSO, int l, int Indent, const char* s)
//{
//	int ls = 0;
//	char* pIndentString = new char[512];
//	CSymbol* pSym = 0;
//	int size = l;
//
//	Act()->IndentString(pIndentString, 512, Indent, ' ');
//	if (s)
//	{
//		size = l - ls;
//		ls += sprintf_s(&pSO[ls], size, "%s%s:", pIndentString, s);
//	}
//	size = l - ls;
//	ls += sprintf_s(&pSO[ls], size, "%s\n", FindItemTypeName(m_ItemType));
//	delete[] pIndentString;
//	return ls;
//}