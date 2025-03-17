#include "pch.h"

CValue::CValue()
{
	m_pSym = 0;
	m_ValType = ValueType::NONE;
	m_ConstantValue = 0;
	m_UpperLOwer = UpperLower::NONE;
	m_pString = 0;
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

bool CValue::Create(ValueType VT)
{
	m_ValType = VT;
	return true;
}

bool CValue::Create(int V)
{
	m_ConstantValue = V;
	m_ValType = ValueType::CONSTANT;
	return true;
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
		if (pSym)
			rV = pSym->GetAddress() + m_ConstantValue;
		else
			rV = 0;
		break;
	}
	return rV;
}

int CValue::GetTotalValue()
{
	int rV = 0;

	switch (GetValueType())
	{
	case ValueType::CONSTANT:
		rV = GetConstVal();
		break;
	case ValueType::SYMBOL:
		rV = GetSymbol()->GetAddress() + GetConstVal();
		break;
	}
	return rV;
}

void CValue::SetString(const char* s)
{
	int l = int(strlen(s)) + 1;

	m_pString = new char[l];
	strcpy_s(m_pString, l, s);
}

void CValue::Add(CValue* pVal)
{
	if (m_ValType == ValueType::SYMBOL)
	{
		switch (pVal->GetValueType())
		{
		case ValueType::CONSTANT:
			m_ConstantValue += pVal->GetConstVal();
			break;
		default:
			fprintf(stderr, "Line:%d Col:%d Right hand side must be a CONST]n",
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			Act()->Exit(77);
			break;
		}
	}
	else if (m_ValType == ValueType::CONSTANT)
	{
		switch (pVal->GetValueType())
		{
		case ValueType::CONSTANT:
			m_ConstantValue += pVal->GetConstVal();
			break;
		default:
			fprintf(stderr, "Line:%d Col:%d Right hand side must be a CONST]n",
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			Act()->Exit(77);
			break;
		}

	}
}

void CValue::Sub(CValue* pVal)
{
	if (m_ValType == ValueType::SYMBOL)
	{
		switch (pVal->GetValueType())
		{
		case ValueType::CONSTANT:
			m_ConstantValue -= pVal->GetConstVal();
			break;
		default:
			fprintf(stderr, "Line:%d Col:%d Right hand side must be a CONST]n",
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			Act()->Exit(77);
			break;
		}
	}
	else if (m_ValType == ValueType::CONSTANT)
	{
		switch (pVal->GetValueType())
		{
		case ValueType::CONSTANT:
			m_ConstantValue -= pVal->GetConstVal();
			break;
		default:
			fprintf(stderr, "Line:%d Col:%d Right hand side must be a CONST]n",
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			Act()->Exit(77);
			break;
		}

	}
}

int CValue::Upper()
{
	int rV = 0;
	CSymbol* pSym = 0;

	pSym = (CSymbol *)GetSymbol();
	if (pSym)
	{
		rV = pSym->GetAddress();
	}
	rV += m_ConstantValue;
	rV &= 0x0ff00;
	rV >>= 8;
	m_ConstantValue = rV;
	m_UpperLOwer = UpperLower::UPPER;
	return rV;
}

int CValue::Lower()
{
	int rV = 0;
	CSymbol* pSym = 0;

	pSym = (CSymbol*)GetSymbol();
	if (pSym)
	{
		rV = pSym->GetAddress();
	}
	rV += m_ConstantValue;
	rV &= 0x0ff;
	m_ConstantValue = rV;
	m_UpperLOwer = UpperLower::LOWER;
	return rV;
}

bool CValue::IsPageZero()
{
	bool rV = false;

	switch (m_ValType)
	{
	case ValueType::CONSTANT:
		if (GetConstVal() > 0 && GetConstVal() < 0x100)
			rV = true;
		break;
	case ValueType::SYMBOL:
		if (GetSymbol())
		{
			if (((CSymbol*)GetSymbol())->GetSection())
			{
				if (((CSymbol*)GetSymbol())->GetSection()->IsPageZero())
					rV = true;
			}
			else
			{
				fprintf(stderr, "CValue::IsPageZero()::Internal Error:Line %d  Col %d",
					Act()->GetParser()->GetLexer()->GetLineNumber(),
					Act()->GetParser()->GetLexer()->GetColunm()
				);
				Act()->Exit(33);
			}
		}
		else
		{
			fprintf(stderr, "CValue::IsPageZero()::Internal Error:Line %d  Col %d",
				Act()->GetParser()->GetLexer()->GetLineNumber(),
				Act()->GetParser()->GetLexer()->GetColunm()
			);
			Act()->Exit(33);
		}
		break;
	}
    return rV;
}
