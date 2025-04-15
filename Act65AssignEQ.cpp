#include "pch.h"

CAct65AssignEQ::CAct65AssignEQ() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_EQ)
{
}

CAct65AssignEQ::~CAct65AssignEQ()
{
}

bool CAct65AssignEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignEQ::Process()
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
	if (pNext)
		return Emit(m_pChildValue, m_pChildValue);
	else
		return nullptr;
}

int CAct65AssignEQ::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignEQ::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignEQ::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
