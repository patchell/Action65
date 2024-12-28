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
	int size;

	if(pObjName)
		ls += sprintf_s(pSO, l, "%s:", pObjName);
	return ls;
}

const char* CObjTypeChain::Types::FindName(Spec T)
{
	return TypesLUT[int(T)].m_pName;
}
