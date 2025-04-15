#include "pch.h"

CAct65SET::CAct65SET() :CAstNode(m_pNodeTyypeName, NodeType::SET)
{
}

CAct65SET::~CAct65SET()
{
}

bool CAct65SET::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SET::Process()
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

void CAct65SET::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
}

int CAct65SET::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

CValue* CAct65SET::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
