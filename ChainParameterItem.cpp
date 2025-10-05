#include "pch.h"

void CChainParameterItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::PARAMETER)))
	{
		m_pParameter = ((CChainParameterItem*)pI)->m_pParameter;
	}
}

int CChainParameterItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	char* pIndentString = new char[512];

	Act()->IndentString(pIndentString, 512, Indent);
	if (s)
		ls += sprintf_s(pSO + ls, l - ls, "%s%s:", pIndentString, s);
	ls += sprintf_s(pSO + ls, l - ls, "%s\n", ItemTypeLUT[int(GetItemType())].m_pName);
	if (m_pParameter)
	{
		m_pParameter->Print(pSO + ls, l - ls, Indent, pIndentString);
	}
	else
	{
		ls += sprintf_s(pSO + ls, l - ls, "%sNo Symbol\n", pIndentString);
	}
	delete[] pIndentString;
	return ls;
}