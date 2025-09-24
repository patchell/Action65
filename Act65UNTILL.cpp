#include "pch.h"

CAct65UNTILL::CAct65UNTILL() :CAstNode(m_pNodeTyypeName, NodeType::UNTIL)
{
}

CAct65UNTILL::~CAct65UNTILL()
{
}

bool CAct65UNTILL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65UNTILL::Process()
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

int CAct65UNTILL::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65UNTILL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65UNTILL::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
