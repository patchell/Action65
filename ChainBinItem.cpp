#include "pch.h"

CChainBinItem::CChainBinItem() :CChainItem(ChainItemType::BIN)
{
	m_pBin = 0;
}

CChainBinItem::~CChainBinItem()
{
}

void CChainBinItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::BIN)))
	{
		m_pBin = ((CChainBinItem*)pI)->m_pBin;
	}
}

bool CChainBinItem::Compare(const char* pName)
{
	bool rV = false;

	if (strcmp(pName,GetSymbol()->GetName()) == 0)
		rV = true;
	return rV;
}

int CChainBinItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;
	char* pIndentString = new char[512];

	Act()->IndentString(pIndentString, 512, Indent, ' ');
	if (s)
	{
		ls += snprintf(&pSO[ls], size - ls, "%s%s\n", pIndentString, s);
	}
	if (GetSymbol())
	{
		ls += GetSymbol()->Print(&pSO[ls], size - ls, Indent + 2, "Symbol:");
	}
	else
	{
		size = l - ls;
		ls += snprintf(&pSO[ls], size, "%s  <No Symbol>\n", pIndentString);
	}
	delete[] pIndentString;
	return ls;
}

void CChainBinItem::Emit(CSection* pSec)
{

}

void CChainBinItem::EmitListing(CSection* pSec)
{
}
