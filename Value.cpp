#include "pch.h"

#define BYTE_SIZE	1
#define WORD_SIZE	2

CValue::CValue()
{
	m_pSym = 0;
	m_ValType = ValueType::NONE;
	m_ConstantValue = 0;
	m_UpperLOwer = UpperLower::NONE;
	m_pString = 0;
	m_pVirtualReg = 0;
	m_AltTypeChain.Create();
	m_pReg = 0;
}

CValue::~CValue()
{
}

bool CValue::Create(CVirtualReg::VREG* pVReg)
{
	m_ValType = ValueType::VIRTUAL_REGISTER;
	m_pVirtualReg = pVReg;
	return false;
}

bool CValue::Create(const char* s)
{
	SetString(s, ValueType::CSTRING);	//default is C-String
	return true;
}

bool CValue::Create(CSymbol* pSym)
{
	//-----------------------------------------------
	// Creates a Value from a symbol
	//-----------------------------------------------
	bool rV = true;

	if (pSym)
	{
		SetSymbol(pSym);
		m_ValType = ValueType::SYMBOL;
	}
	else
	{
		//-------------------------
		// Symbol was NULL, so fail
		//-------------------------
		rV = false;
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_SYMBOL_NULL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error: Value Create Error:NULL Symbol\n"
		);
		throw(ThrownException);
	}
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

bool CValue::Create(CReg* pReg)
{
	m_pReg = pReg;
	m_ValType = ValueType::REG;
	return true;
}

void CValue::SetSymbol(CSymbol* pSym)
{
	m_pSym = pSym;
	if (pSym)
	{
		m_ValType = ValueType::SYMBOL;
	}
}

CSymbol* CValue::GetSymbol()
{
	CSymbol* pSym = 0;

	switch (m_ValType)
	{
	case ValueType::SYMBOL:
		pSym = m_pSym;
		break;
	case ValueType::VIRTUAL_REGISTER:
		pSym = m_pVirtualReg->GetSymbol();
		break;
	}
	return pSym;
}

CTypeChain* CValue::GetTypeChain()
{
	CTypeChain* pTC = 0;

	switch (m_ValType)
	{
	case ValueType::VIRTUAL_REGISTER:
	case ValueType::SYMBOL:
		pTC = GetSymbol()->GetTypeChain();
		break;
	case ValueType::CONSTANT:
	case ValueType::ADDRESS_OF:
		pTC = &m_AltTypeChain;
		break;
	case ValueType::REG:
		pTC = m_pReg->GetTypeChain();
		break;
	}
	return pTC;
}

void CValue::SetTypeChain(CTypeChain* pTC)
{
	switch (GetValueType())
	{
	case ValueType::SYMBOL:
	case ValueType::VIRTUAL_REGISTER:
		GetSymbol()->CreateTypeDefChain();
		GetSymbol()->GetTypeChain()->CopyTypeChain(pTC);
		break;
	case ValueType::CONSTANT:
		break;
	}
}

char* CValue::GetName()
{
	char* pName = 0;
	if (GetSymbol())
		pName = GetSymbol()->GetName();
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:No Symbol Attatch to Value\n"
		);
		throw(ThrownException);
	}
    return pName;
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
	case ValueType::VIRTUAL_REGISTER:
		rV = m_ConstantValue;
		break;
	case ValueType::ASTRING:
	case ValueType::CSTRING:
	case ValueType::STRING:
		rV = m_ConstantValue;
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

void CValue::SetString(const char* s, ValueType Type)
{
	int l = int(strlen(s));

	m_pString = new char[l + 1];
	strncpy_s(m_pString, l + 1, s, l +1);
	m_ConstantValue = l;	//this is the string length
	m_ValType = Type;
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
	case ValueType::VIRTUAL_REGISTER:
		if (GetSymbol())
		{
			if (((CSymbol*)GetSymbol())->GetSection())
			{
				if (((CSymbol*)GetSymbol())->GetSection()->IsPageZero())
					rV = true;
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

int CValue::SizeOf()
{
	int rV = 0;
	CTypeChain* pTypeChain = 0;

	pTypeChain = GetTypeChain();
	if (!pTypeChain)
		printf("Oh-Oh\n");
	if (pTypeChain->Is(CObjTypeChain::Spec::POINTER))
		rV = 2;
	else if (pTypeChain->IsByte())
		rV = 1;
	else if (pTypeChain->IsWord())
		rV = 2;
	return rV;
}

void CValue::SetAddress(int Addr)
{
	if(m_pSym)
		m_pSym->SetAddress(Addr);
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CValue has no Symbol\n"
		);
		throw(ThrownException);
	}
}

void CValue::SetResolved(bool bRes)
{
	if(m_pSym)
	{
		if(bRes)
			m_pSym->SetResolved();
		else
			m_pSym->SetUnResolved();
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CValue has no Symbol\n"
		);
		throw(ThrownException);
	}
}

void CValue::BackFillUnresolved()
{
	if (m_pSym)
		m_pSym->BackFillUnresolved();
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"CValue has no Symbol\n"
		);
		throw(ThrownException);
	}
}