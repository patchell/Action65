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

void CWhereSymbolIsUsed::SetAddress(int a)
{
	m_Address = a;
//	fprintf(
//		Act()->LogFile(),
//		"Set Unresolved Symbol Location : %04x\n",
//		a
//	);
}
