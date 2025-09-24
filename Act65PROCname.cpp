#include "pch.h"

CAct65PROCname::CAct65PROCname() :CAstNode(m_pNodeTyypeName, NodeType::PROC_NAME)
{
}

CAct65PROCname::~CAct65PROCname()
{
}

bool CAct65PROCname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65PROCname::Process()
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

int CAct65PROCname::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	if (GetSymbol())
	{
		if (GetSymbol()->GetName())
		{
			int size;
			char* z = GetSymbol()->GetName();
			size = Strlen - l;
			l += sprintf_s(&s[l], size, " - \'%s\'", z);
		}
	}
	return l;
}

void CAct65PROCname::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PROCname::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
