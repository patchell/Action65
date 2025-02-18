#include "pch.h"

CAct65ASMstatement::CAct65ASMstatement() :CAstNode(NodeType::ASM_STATEMENT)
{
}

CAct65ASMstatement::~CAct65ASMstatement()
{
}

bool CAct65ASMstatement::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ASMstatement::Process()
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

int CAct65ASMstatement::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65ASMstatement::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
