#include "pch.h"

CAct65ForSTEP::CAct65ForSTEP() :CAstNode(m_pNodeTyypeName, NodeType::FOR_STEP)
{
}

CAct65ForSTEP::~CAct65ForSTEP()
{
}

bool CAct65ForSTEP::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ForSTEP::Process()
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

int CAct65ForSTEP::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ForSTEP::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ForSTEP::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
