#include "pch.h"

CWhereSymbolIsUsed::CWhereSymbolIsUsed() :CBin(CBin::BinType::WHERE_USED)
{
	m_Address = 0;
	m_pSection = 0;
	m_UnResRefType = UnResolvedType::ABSOLUTE_REFERENCE;
}

CWhereSymbolIsUsed::~CWhereSymbolIsUsed()
{
}

bool CWhereSymbolIsUsed::Create()
{
	return CBin::Create();
}

bool CWhereSymbolIsUsed::Compare(const char* name, int scope)
{
	bool rV = false;

	if (strcmp(GetName(), name) == 0)
	{
		rV = true;
	}
	return rV;
}

void CWhereSymbolIsUsed::SetAddress(int a)
{
	m_Address = a;
//	fprintf(
//		Act()->LogFile(),
//		"Set Unresolved Symbol Location : %04x\n",
//		a
//	);
}
