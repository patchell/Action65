#include "pch.h"

CChainValueItem::CChainValueItem() :CChainItem(ChainItemType::VALUE)
{
	m_pValue = 0;
	m_Location = 0;
}

CChainValueItem::~CChainValueItem()
{
}

void CChainValueItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::VALUE)))
	{
		m_pValue = ((CChainValueItem*)pI)->m_pValue;
		m_Location = ((CChainValueItem*)pI)->m_Location;
	}
}

bool CChainValueItem::Compare(const char* pName)
{
	return false;
}

int CChainValueItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;
	char* pIndentString = new char[512];

	Act()->IndentString(pIndentString, 512, Indent, ' ');
	if (s)
	{
		ls += snprintf(
			pSO + ls,
			size - ls,
			"%sCChainValueItem: %s\n",
			pIndentString,
			s
		);
	}
	else
	{
		ls += snprintf(
			pSO + ls,
			size - ls,
			"%sCChainValueItem:\n",
			pIndentString
		);
	}
	if (m_pValue)
	{
		ls += m_pValue->Print(
			pSO + ls,
			size - ls,
			Indent + 2,
			"Value"
		);
	}
	else
	{
		size = l - ls;
		ls += snprintf(
			pSO + ls,
			size,
			"%s  Value: NULL\n",
			pIndentString
		);
	}
	size = l - ls;
	ls += snprintf(
		pSO + ls,
		size,
		"%s  Location: 0x%04x\n",
		pIndentString,
		m_Location
	);
	delete[] pIndentString;
	return ls;
}

void CChainValueItem::EmitListing(CSection* pSec)
{
}

