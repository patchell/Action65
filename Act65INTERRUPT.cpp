#include "pch.h"

CAct65INTERRUPT::CAct65INTERRUPT() :CAstNode(m_pNodeTyypeName, NodeType::INTERRUPT)
{
}

CAct65INTERRUPT::~CAct65INTERRUPT()
{
}

bool CAct65INTERRUPT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65INTERRUPT::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

//	fprintf(Act()->LogFile(), "Process %s Node:%d\n", GetNodeName(), GetID());
	Emit(0, 0);
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
	return nullptr;
}

int CAct65INTERRUPT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65INTERRUPT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65INTERRUPT::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
