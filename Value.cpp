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

	if (pSym)
	{
		SetSymbol(pSym);
		m_ValType = ValueType::SYMBOL;
	}
	else
		m_ValType = ValueType::CONSTANT;
    return rV;
}

void CValue::SetSymbol(CBin* pSym)
{
	m_pSym = pSym;
	if (pSym)
		m_ValType = ValueType::SYMBOL;
}

int CValue::GetConstVal()
{
	int rV = 0;
	CSymbol* pSym = 0;

	switch (m_ValType)
	{
	case ValueType::NONE:
		break;
	case ValueType::CONSTANT:
		rV = m_ConstantValue;
		break;
	case ValueType::SYMBOL:
		pSym = (CSymbol*)GetSymbol();
		rV = pSym->GetAddress();
		break;
	}
	return rV;
}
