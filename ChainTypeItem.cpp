#include "pch.h"

CChainTypeItem::CChainTypeItem() :CChainItem(CChainItem::ChainItemType::TYPE)
{
	m_SpecType = Spec::NONE;
}

CChainTypeItem::~CChainTypeItem()
{
}

bool CChainTypeItem::Create()
{
	return false;
}

void CChainTypeItem::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::TYPE)))
	{
		m_SpecType = ((CChainTypeItem*)pI)->m_SpecType;
	}
}

bool CChainTypeItem::Compare(const char* pName)
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

int CChainTypeItem::Print(char* pSO, int l)
{
	int ls = 0;
	const char* pObjName = FindName(m_SpecType);

	if(pObjName)
		ls += sprintf_s(pSO, l, "%s:", pObjName);
	return ls;
}

bool CChainTypeItem::IsFundamentalType()
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

bool CChainTypeItem::Is(Spec SpecType) const
{
	bool rV = false;

	if (m_SpecType == SpecType)
		rV = true;
	return rV;
}

const char* CChainTypeItem::Types::FindName(Spec T)
{
	return TypesLUT[int(T)].m_pName;
}

int CChainTypeItem::Print(char* pSO, int l, int Indent, const char* s)
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