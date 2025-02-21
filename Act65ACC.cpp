#include "pch.h"

CAct65ACC::CAct65ACC() :CAstNode(m_pNodeTyypeName, NodeType::AREG)
{
}

CAct65ACC::~CAct65ACC()
{
}

bool CAct65ACC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild,pNext);
}

CValue* CAct65ACC::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
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
