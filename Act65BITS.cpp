#include "pch.h"

CAct65BITS::CAct65BITS() :CAstNode(m_pNodeTyypeName, NodeType::BITS)
{
}

CAct65BITS::~CAct65BITS()
{
}

bool CAct65BITS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BITS::Process()
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

int CAct65BITS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BITS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BITS::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
