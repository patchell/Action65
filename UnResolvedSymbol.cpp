#include "pch.h"

CUnResolvedSymbol::CUnResolvedSymbol():CBin(CBin::BinType::UNRESOLVED)
{
	m_Address = 0;
	m_pSection = 0;
	m_UnResRefType = CWhereSymbolIsUsed::UnResolvedType::ABSOLUTE_REFERENCE;
}

CUnResolvedSymbol::~CUnResolvedSymbol()
{
}

bool CUnResolvedSymbol::Create()
{
	return CBin::Create();
}

void CUnResolvedSymbol::SetAddress(unsigned a)
{
	m_Address = a;
	fprintf(
		Act()->LogFile(),
		"Set Unresolved Symbol Location : %04x\n",
		a
	);
}
