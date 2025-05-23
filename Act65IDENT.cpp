#include "pch.h"

CAct65IDENT::CAct65IDENT() :CAstNode(m_pNodeTyypeName, NodeType::IDENT)
{
}

CAct65IDENT::~CAct65IDENT()
{
}

bool CAct65IDENT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65IDENT::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValue = 0;

	fprintf(Act()->LogFile(), "Process %s %s Node:%d\n", GetNodeName(), GetSymbol()->GetName(), GetID());
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
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65IDENT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag); 
	if (GetValue())
	{
		if (GetValue()->GetSymbol())
		{
			if (GetValue()->GetSymbol()->GetName())
			{
				int size = Strlen - l;
				l += sprintf_s(&s[l], size, ": %s", GetValue()->GetSymbol()->GetName());
			}
		}
	}
	return l;
}

void CAct65IDENT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	int x = 0;
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IDENT::Emit(CValue* pVc, CValue* pVn)
{
	fprintf(LogFile(), "EMIT:IDENT:ID=%d:%s\n", 
		GetID(), 
		GetSymbol()->GetName()
	);
    return GetValue();
}
