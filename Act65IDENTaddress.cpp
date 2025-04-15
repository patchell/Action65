#include "pch.h"

CAct65IDENTaddress::CAct65IDENTaddress() :CAstNode(m_pNodeTyypeName, NodeType::IDENT_ADDRESS)
{
}

CAct65IDENTaddress::~CAct65IDENTaddress()
{
}

bool CAct65IDENTaddress::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return false;
}

CValue* CAct65IDENTaddress::Process()
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

int CAct65IDENTaddress::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65IDENTaddress::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IDENTaddress::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
