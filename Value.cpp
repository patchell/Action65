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
	if (m_pString)
	{
		delete[] m_pString;
		m_pString = 0;
	}
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
			"CValue::Create: Internal Error: Value Create Error:NULL Symbol\n"
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

void CValue::Copy(CValue* pV)
{
	m_ValType = pV->m_ValType;
	m_ConstantValue = pV->m_ConstantValue;
	m_UpperLOwer = pV->m_UpperLOwer;
	m_pSym = pV->m_pSym;
	m_pReg = pV->m_pReg;
	m_pVirtualReg = pV->m_pVirtualReg;
	if (pV->m_pString)
	{
		int l = (int)strlen(pV->m_pString) + 1;
		m_pString = new char[l];
		strcpy_s(m_pString, l, pV->m_pString);
	}
	else
	{
		m_pString = 0;
	}
	m_AltTypeChain.CopyTypeChain(&pV->m_AltTypeChain);
}

int CValue::Print(char* pSO, int l, int Indent, const char* s)
{
	// Implementation of Print function
	int ls = 0;

	ls = GetSymbol()->Print(pSO, l, Indent, s);
	return ls;
}

void CValue::SetSymbol(CSymbol* pSym)
{
	m_pSym = pSym;
	if (pSym)
	{
		m_ValType = ValueType::SYMBOL;
	}
}

CChainTypeSpec* CValue::GetTypeChain()
{
	CChainTypeSpec* pTC = 0;

	switch (m_ValType)
	{
	case ValueType::VIRTUAL_REGISTER:
	case ValueType::SYMBOL:
		if (GetSymbol())
		{
			pTC = GetSymbol()->GetTypeChain();
		}
		else
		{
			ThrownException.SetXCeptType(Exception::ExceptionType::CVALUE_NO_SYMBOL);
			sprintf_s(
				ThrownException.GetErrorString(),
				ThrownException.GetMaxStringLen(),
				"CValue::GetTypeChain(): Internal Error:No Symbol Attached to Value\n"
			);
			throw(ThrownException);
		}
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

void CValue::SetTypeChain(CChain* pTC)
{
	switch (GetValueType())
	{
	case ValueType::SYMBOL:
	case ValueType::VIRTUAL_REGISTER:
		GetSymbol()->CreateTypeDefFieldChain();
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
			"CValue::GetName(): Internal Error:No Symbol Attached to Value\n"
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
				Act()->GetParser()->GetLexer()->GetColumn()
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
				Act()->GetParser()->GetLexer()->GetColumn()
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
				Act()->GetParser()->GetLexer()->GetColumn()
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
				Act()->GetParser()->GetLexer()->GetColumn()
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
				Act()->GetParser()->GetLexer()->GetColumn()
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
	CChainTypeSpec* pTypeChain = 0;

	pTypeChain = GetTypeChain();
	if (!pTypeChain)
		printf("Oh-Oh\n");
	if (pTypeChain->Is(CChainTypeSpecItem::Spec::ARRAY))
	{
		rV = GetConstVal() * pTypeChain->SizeOf();
	}
	else if (pTypeChain->Is(CChainTypeSpecItem::Spec::TYPEDEF))
	{
		CValue* pFieldValue = 0;
		CChain* pTypeDefChain = 0;
		CChainItem* pItem;

		pTypeDefChain = GetTypeTypeDefFieldChain();
		pItem = pTypeDefChain->GetHead();
		while (pItem)
		{
			if(pItem->Is(CChainItem::ChainItemType::VALUE))
			{
				pFieldValue = ((CChainValueItem*)pItem)->GetValue();
				rV += pFieldValue->SizeOf();
			}
			pItem = pItem->GetNext();
		}
	}
	else
	{
		rV = pTypeChain->SizeOf();
	}
	return rV;
}

int CValue::SizeOfTypeDef()
{
	int Size = 0;
	CChainItem* pItem = 0;
	CChain* pChain = 0;


	if (GetTypeChain())
	{
		pChain = this->GetTypeTypeDefFieldChain();
		pItem = pChain->GetHead();
		while (pItem)
		{
			if (pItem->Is(CChainItem::ChainItemType::VALUE))
			{
				Size += ((CChainValueItem*)pItem)->GetValue()->SizeOf();
			}
			pItem = pItem->GetNext();
		}
	}
	return Size;
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
			"CValue::SetAddress: Value has no Symbol\n"
		);
		throw(ThrownException);
	}
}

int CValue::GetAddress()
{
	int Address = 0;
	if(m_pSym)
		Address = m_pSym->GetAddress();
	Address += m_ConstantValue;
	return Address;
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
			"CValue::SetResolved: Value has no Symbol\n"
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
			"CValue::BackFillUnresolved: Value has no Symbol\n"
		);
		throw(ThrownException);
	}
}

CChain* CValue::GetTypeTypeDefFieldChain()
{
	CChain* pChain = nullptr;

	if (GetSymbol())
	{
		pChain = GetSymbol()->GetTypeDefFieldChain();
	}
	return pChain;
}
