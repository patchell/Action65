#include "pch.h"

CAct65IDENT::CAct65IDENT() :CAstNode(m_pNodeTyypeName, NodeType::IDENT)
{
}

CAct65IDENT::~CAct65IDENT()
{
}

bool CAct65IDENT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym, CSection* pSec)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym, pSec);
	return rV;
}

CValue* CAct65IDENT::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		m_pChildValue = pChild->Process();
	}
	while (pNext)
	{
		m_pNextValue = pNext->Process();
		pNext = pNext->GetNext();
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65IDENT::Print(char* s, int Strlen, int Indent, const char* pAuxStr, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(s, Strlen, Indent, pAuxStr, pbNextFlag);
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				int size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	return l;
}

void CAct65IDENT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	int x = 0;
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IDENT::Emit(CValue* pVc, CValue* pVn)
{
	CChainTypeSpec* pTypeSpecChain = 0;
	CChainStorageItem* pStorageItem = 0;

	int ObjectSize = 0;

	if (GetValue())
	{
		pTypeSpecChain = GetValue()->GetTypeChain();
		if (pTypeSpecChain)
		{
			switch(GetValue()->GetValueType())
			{
			case CValue::ValueType::SYMBOL:
				ObjectSize = pTypeSpecChain->SizeOf();
				pStorageItem = new CChainStorageItem();
				pStorageItem->Create(ObjectSize);
				GetValue()->SetAddress(GetSection()->GetLocationCounter());
				GetSection()->IncrementLocationCounterBy(ObjectSize);
				pStorageItem->SetLabelValue(GetValue());
				GetSection()->GetSectionDataChain()->AddToTail(pStorageItem);
				break;
			case CValue::ValueType::VIRTUAL_REGISTER:
				break;
			case CValue::ValueType::CONSTANT:
				break;
			case CValue::ValueType::ADDRESS_OF:
				pTypeSpecChain = GetValue()->GetTypeChain();
				break;
			case CValue::ValueType::REG:
				pTypeSpecChain = GetValue()->GetTypeChain();
				break;
			}
		}
	}
	else
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::CODEGEN_NO_SECTION);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Code Generation Line %d: CAct65VARLOCAL Value is NULL\n",
			GetLine()
		);
		throw(ThrownException);
	}
    return GetValue();
}
