#include "pch.h"

CValue::CValue()
{
	m_pSym = 0;
	m_ValType = ValueType::NONE;
	m_ConstantValue = 0;
}

CValue::~CValue()
{
}

bool CValue::Create(const char* s)
{
	SetString(s);
	m_ValType = ValueType::STRING;
	return true;
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

bool CValue::Create(int V)
{
	m_ConstantValue = V;
	m_ValType = ValueType::CONSTANT;
	return false;
}

void CValue::SetSymbol(CBin* pSym)
{
//	if(int(pSym) > 8)
//		fprintf(stderr, "Symbol:  %s  :%x\n", pSym->GetName(),int(pSym));
//	else if (int(pSym) > 0)
//	{
//		fprintf(stderr, "Opps\n");
//		Act()->Exit(123);
//	}
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

void CValue::SetString(const char* s)
{
	int l = strlen(s) + 1;

	m_pString = new char[l];
	strcpy_s(m_pString, l, s);
}
