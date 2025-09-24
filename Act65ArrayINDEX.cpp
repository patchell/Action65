#include "pch.h"

CAct65ArrayINDEX::CAct65ArrayINDEX() :CAstNode(m_pNodeTyypeName, NodeType::ARRAY_INDEX)
{
}

CAct65ArrayINDEX::~CAct65ArrayINDEX()
{
}

bool CAct65ArrayINDEX::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65ArrayINDEX::Process()
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
	if (pNext)
	{
		ThrownException.SetXCeptType(Exception::ExceptionType::INTERNAL_SYMBOL_NULL);
		sprintf_s(
			ThrownException.GetErrorString(),
			ThrownException.GetMaxStringLen(),
			"Internal Error:CAct65ArrayINDEX cannot have a NEXT node Line:%d\n", 
			GetLine()
		);
		throw(ThrownException);
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65ArrayINDEX::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
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

void CAct65ArrayINDEX::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ArrayINDEX::Emit(CValue* pVc, CValue* pVn)
{
	//---------------------------------
	// pVc...... Index Value
	// pVn.......Should be NULL
	// For now, the value will end up
	// in the XREG
	//---------------------------------
	CValue* pRetValue = 0;
	CInstruction* pOpCode = 0;
	CValue* pLabel;

	pLabel = GetCodeGen()->GetPendingLabel();
	switch (pVc->GetValueType())
	{
	case CValue::ValueType::REG:
		switch (pVc->GetRegister()->GetType())
		{
		case CReg::RegType::A:
			pOpCode = new CInstruction;
			pOpCode->GenInstruction(Token::TAX, AdrModeType::IMPLIED, 0, pLabel, GetSection()->GetLocationCounter());
			GetSection()->AddInstruction(pOpCode);
			pVc->GetRegister()->SetType(CReg::RegType::X);
			break;
		case CReg::RegType::X:
			break;
		case CReg::RegType::Y:
			break;
		}
		break;
	case CValue::ValueType::VIRTUAL_REGISTER:
	case CValue::ValueType::SYMBOL:
		break;
	case CValue::ValueType::CONSTANT:
		break;
	}
    return pRetValue;
}
