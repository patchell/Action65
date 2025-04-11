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
	CValue* pV = 0;
	CValue* pVChild = 0;
	CValue* pVNext = 0;
	CAstNode* pNodeChild = 0;
	CAstNode* pNodeNext = 0;

	fprintf(LogFile(), "Process ASSIGNMENT ID = %d\n", GetID());

	pNodeChild = GetChild();
	if (GetChild())
		pNodeNext = GetChild()->GetNext();
	if (pNodeChild)
		pVChild = pNodeChild->Process();
	if (pNodeNext)
		pVNext = pNodeNext->Process();
	Emit(pVChild, pVNext);
	return pV;
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
