#include "pch.h"

CChainTypeSpecItem::CChainTypeSpecItem() :CChainItem(CChainItem::ChainItemType::TYPE)
{
	m_SpecType = Spec::NONE;
}

CChainTypeSpecItem::~CChainTypeSpecItem()
{
}

bool CChainTypeSpecItem::Create()
{
	return true;
}

void CChainTypeSpecItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::TYPE)))
	{
		m_SpecType = ((CChainTypeSpecItem*)pI)->m_SpecType;
	}
}

bool CChainTypeSpecItem::Compare(const char* pName)
{
	bool rV = false;
	const char* pObjName = FindName(m_SpecType);
	if (pObjName)
	{
		if (strcmp(pObjName, pName) == 0)
			rV = true;
	}
	return rV;
}

int CChainTypeSpecItem::Print(char* pSO, int l)
{
	int ls = 0;
	const char* pObjName = FindName(m_SpecType);

	if(pObjName)
		ls += sprintf_s(pSO, l, "%s:", pObjName);
	return ls;
}

bool CChainTypeSpecItem::IsFundamentalType()
{
	bool rV = false;

	switch (m_SpecType)
	{
	case Spec::BOOL:
	case Spec::BYTE:
	case Spec::CARD:
	case Spec::CHAR:
	case Spec::INT:
		rV = true;
		break;
	}
    return rV;
}

bool CChainTypeSpecItem::Is(Spec SpecType) const
{
	bool rV = false;

	if (m_SpecType == SpecType)
		rV = true;
	return rV;
}

const char* CChainTypeSpecItem::Types::FindName(Spec T)
{
	return TypesLUT[int(T)].m_pName;
}

int CChainTypeSpecItem::Print(char* pSO, int l, int Indent, const char* s)
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
	size -= ls;
	const char* pObjName = FindName(m_SpecType);
	if (pObjName)
	{
		size -= ls;
		ls += sprintf_s(&pSO[ls], size, "%sType:%s\n", pIndentString, pObjName);
	}
	else
	{
		size -= ls;
		ls += sprintf_s(&pSO[ls], size, "%sType:Unknown\n", pIndentString);
	}
	delete[] pIndentString;
	return ls;
}