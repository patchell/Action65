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
	int size = l;
	char* pIndentString = new char[512];

	Act()->IndentString(pIndentString, 512, Indent, ' ');
	if (s)
	{
		size -= ls;	
		ls += sprintf_s(&pSO[ls], size, "%s%s:", pIndentString, s);
	}
	size -= ls;
	ls += sprintf_s(&pSO[ls], size, "%s\n", ItemTypeLUT[int(GetItemType())].m_pName);
	if (m_pParameter)
	{
		size -= ls;
		m_pParameter->Print(&pSO[ls], size, Indent, pIndentString);
	}
	else
	{
		size -= ls;
		ls += sprintf_s(&pSO[ls], size, "%sNo Symbol\n", pIndentString);
	}
	delete[] pIndentString;
	return ls;
}