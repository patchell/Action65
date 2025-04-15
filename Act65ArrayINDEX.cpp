#include "pch.h"

CAct65ArrayINDEX::CAct65ArrayINDEX() :CAstNode(m_pNodeTyypeName, NodeType::ARRAY_INDEX)
{
}

CAct65ArrayINDEX::~CAct65ArrayINDEX()
{
}

bool CAct65ArrayINDEX::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ArrayINDEX::Process()
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
		fprintf(Act()->LogFile(), "Internal Error:CAct65ArrayINDEX cannot have a NEXT node Line:%d\n", GetLine());
		Act()->Exit(2);
	}
	return Emit(m_pChildValue, m_pChildValue);
}

int CAct65ArrayINDEX::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65ArrayINDEX::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ArrayINDEX::Emit(CValue* pVc, CValue* pVn)
{
    return pVc;
}
