#include "pch.h"

CAct65SUB::CAct65SUB() :CAstNode(m_pNodeTyypeName, NodeType::SUBTRACT)
{
}

CAct65SUB::~CAct65SUB()
{
}

bool CAct65SUB::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SUB::Process()
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
	if (pNext->GetNext())
	{
		m_pResultValue = pNext->GetNext()->Process();
	}
	if (m_pResultValue)
		pValue = AltEmit(m_pChildValue, m_pNextValue, m_pResultValue);
	else
		pValue = Emit(m_pChildValue, m_pNextValue);
	return pValue;
}

int CAct65SUB::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SUB::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65SUB::Emit(CValue* pVc, CValue* pVn)
{
	return Act()->GetCodeGenUtils()->EmitBinaryOp(Token::SBC, pVc, pVn, 0, GetSection());
}

CValue* CAct65SUB::AltEmit(CValue* pVc, CValue* pVn, CValue* pVr)
{
	return Act()->GetCodeGenUtils()->EmitBinaryOp(Token::SBC, pVc, pVn, pVr, GetSection());
}
