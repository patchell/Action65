#include "pch.h"

CObjType::CObjType()
{
	m_SpecType = Spec::NONE;
	m_pNext = 0;
	m_pPrev = 0;
}

CObjType::~CObjType()
{
}

bool CObjType::Create()
{
	return false;
}
