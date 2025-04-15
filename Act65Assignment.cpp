#include "pch.h"

CAct65Assignment::CAct65Assignment() :CAstNode(m_pNodeTyypeName, NodeType::BASE)
{
}

CAct65Assignment::~CAct65Assignment()
{
}

bool CAct65Assignment::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Assignment::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
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
		m_pNextValue = pNext->Process();
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65Assignment::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Assignment::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Assignment::Emit(CValue* pVc, CValue* pVn)
{
	//----------------------------------------------
	// Value to store (assign to) is in pVc
	// Value to store to is pVn
	//----------------------------------------------
	CAct65Opcode* pOpCode = 0;

	switch (pVc->GetValueType())	// from here
	{
	case CValue::ValueType::CONSTANT:
		break;
	case CValue::ValueType::AREG:
		pOpCode = new CAct65Opcode;
//		pOpCode->PrepareInstruction(Token::STA,)
		break;
	case CValue::ValueType::XREG:
		break;
	case CValue::ValueType::YREG:
		break;
	case CValue::ValueType::SYMBOL:
		break;
	case CValue::ValueType::VIRTUAL_REGISTER:
		break;
	default:
		break;
	}
    return nullptr;
}
