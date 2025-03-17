#include "pch.h"

CAct65ASMstatement::CAct65ASMstatement() :CAstNode(m_pNodeTyypeName, NodeType::ASM_STATEMENT)
{
}

CAct65ASMstatement::~CAct65ASMstatement()
{
}

bool CAct65ASMstatement::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ASMstatement::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
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

CValue* CAct65ASMstatement::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
