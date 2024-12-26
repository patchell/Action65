#include "pch.h"

CValue::CValue()
{
	m_pSym = 0;
}

CValue::~CValue()
{
}

bool CValue::Create(CBin* pSym)
{
	bool rV = true;

	SetSymbol(pSym);
    return rV;
}

bool CValue::AsmInstruction::Create()
{
	bool rV = true;
    return rV;
}
