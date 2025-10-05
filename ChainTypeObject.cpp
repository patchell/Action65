#include "pch.h"

CChainTypeObject::CChainTypeObject() :CChainItem(CChainItem::ChainItemType::TYPE)
{
	m_SpecType = Spec::NONE;
}

CChainTypeObject::~CChainTypeObject()
{
}

bool CChainTypeObject::Create()
{
	return false;
}

void CChainTypeObject::Copy(CChainItem* pI)
{
	if (pI && (pI->Is(CChainItem::ChainItemType::TYPE)))
	{
		m_SpecType = ((CChainTypeObject*)pI)->m_SpecType;
	}
}

bool CChainTypeObject::Compare(const char* pName)
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

int CChainTypeObject::Print(char* pSO, int l)
{
	int ls = 0;
	const char* pObjName = FindName(m_SpecType);

	if(pObjName)
		ls += sprintf_s(pSO, l, "%s:", pObjName);
	return ls;
}

bool CChainTypeObject::IsFundamentalType()
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

bool CChainTypeObject::Is(Spec SpecType) const
{
	bool rV = false;

	if (m_SpecType == SpecType)
		rV = true;
	return rV;
}

const char* CChainTypeObject::Types::FindName(Spec T)
{
	return TypesLUT[int(T)].m_pName;
}
