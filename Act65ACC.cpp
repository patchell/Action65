#include "pch.h"

CAct65ACC::CAct65ACC() :CAstNode(m_pNodeTyypeName, NodeType::AREG)
{
}

CAct65ACC::~CAct65ACC()
{
}

bool CAct65ACC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	bool rV = true;
	rV = CAstNode::Create(pChild, pNext, pSym);
	return rV;
}

CValue* CAct65ACC::Process()
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

int CAct65ACC::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65ACC::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ACC::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
