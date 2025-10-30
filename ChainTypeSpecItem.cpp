#include "pch.h"

CChainTypeSpecItem::CChainTypeSpecItem() :CChainItem(CChainItem::ChainItemType::TYPESPEC)
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
	if (pI && (pI->Is(CChainItem::ChainItemType::TYPESPEC)))
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

bool CChainTypeSpecItem::IsScopeType()
{
	bool rV = false;

	switch (m_SpecType)
	{
	case Spec::PROC_SCOPE:
	case Spec::FUNC_SCOPE:
	case Spec::INTERRUPT_SCOPE:
	case Spec::LOCAL_SCOPE:
	case Spec::GLOBAL_SCOPE:
		[[fallthrough]];
	case Spec::PARAM_SCOPE:
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
	const char* rV = nullptr;

	for (int i = 0; TypesLUT[i].m_pName && !rV; ++i)
	{
		if (TypesLUT[i].m_SpecType == T)
			rV = TypesLUT[i].m_pName;
	}
	return rV;
}

int CChainTypeSpecItem::Print(char* pSO, int l, int Indent, const char* s)
{
	int ls = 0;
	int size = l;
	char* pIndentString = new char[512];

	Act()->IndentString(pIndentString, 512, Indent, ' ');
	if (s && (strlen(s) > 0))
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, "%s%s:\n", pIndentString, s);
		Act()->IndentString(pIndentString, 512, Indent+2, ' ');
	}
	size = l - ls;
	ls += sprintf_s(&pSO[ls], size, "%sChain Item Type:%s:", pIndentString, FindItemTypeName(GetItemType()));
	size = l - ls;
	const char* pObjName = FindName(m_SpecType);
	if (pObjName)
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, " Type:%s\n", pObjName);
	}
	else
	{
		size = l - ls;
		ls += sprintf_s(&pSO[ls], size, " Type:<Unknown>\n");
	}
	delete[] pIndentString;
	return ls;
}