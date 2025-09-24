#include "pch.h"

CAct65BIT::CAct65BIT() :CAstNode(m_pNodeTyypeName, NodeType::BIT)
{
}

CAct65BIT::~CAct65BIT()
{
}

bool CAct65BIT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65BIT::Process()
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

int CAct65BIT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BIT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BIT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
