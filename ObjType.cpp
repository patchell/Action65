#include "pch.h"

CObjTypeChain::CObjTypeChain()
{
	m_SpecType = Spec::NONE;
	m_pNext = 0;
	m_pPrev = 0;
}

CObjTypeChain::~CObjTypeChain()
{
}

bool CObjTypeChain::Create()
{
	return false;
}

int CObjTypeChain::Print(char* pSO, int l)
{
	int ls = 0;
	const char* pObjName = FindName(m_SpecType);

	if(pObjName)
		ls += sprintf_s(pSO, l, "%s:", pObjName);
	return ls;
}

bool CObjTypeChain::IsFundamentalType()
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

bool CObjTypeChain::Is(Spec SpecType) const
{
	bool rV = false;

	if (m_SpecType == SpecType)
		rV = true;
	return rV;
}

const char* CObjTypeChain::Types::FindName(Spec T)
{
	return TypesLUT[int(T)].m_pName;
}
