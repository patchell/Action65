#include "pch.h"


CChainParameterItem::CChainParameterItem() :CChainItem(CChainItem::ChainItemType::PARAMETER)
{
	m_pValue = nullptr;
}

bool CChainParameterItem::Create(CSymbol* pSym)
{
	if (m_pValue)
	{
		m_pValue->SetSymbol(pSym);
	}
	else
	{
		m_pValue = new CValue;
		m_pValue->Create(pSym);
	}
	return true;
}

bool CChainParameterItem::Create(CValue* pVal)
{
	m_pValue = pVal;
	return true;
}

bool CChainParameterItem::Is(const char* pName)
{
	bool rV = false;
	if (GetName())
	{
		if (strcmp(pName, GetName()) == 0)
			rV = true;
	}
	return rV;
}

bool CChainParameterItem::Compare(const char* pName)
{
	return Is(pName);
}

void CChainParameterItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::PARAMETER)))
	{
		m_pValue = ((CChainParameterItem*)pI)->m_pValue;
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
		size = l - ls;	
		ls += sprintf_s(&pSO[ls], size, "%s%s:", pIndentString, s);
	}
	size = l - ls;
	ls += sprintf_s(&pSO[ls], size, "%s\n", FindItemTypeName(GetItemType()));
	if (m_pValue)
	{
		size = l - ls;
		ls = m_pValue->Print(&pSO[ls], size, Indent, pIndentString);
	}
	else
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%sNo Symbol\n", pIndentString);
	}
	delete[] pIndentString;
	return ls;
}

char* CChainParameterItem::GetName()
{
	char* pName = nullptr;

	if (m_pValue)
	{
		pName = m_pValue->GetName();
	}
	return pName;
}

CSymbol* CChainParameterItem::GetSymbol()
{
	CSymbol* pSym = nullptr;
	if (m_pValue)
	{
		pSym = m_pValue->GetSymbol();
	}
	return pSym;
}

void CChainParameterItem::Emit(CSection* pSec)
{

}

void CChainParameterItem::EmitListing(CSection* pSec)
{
}
