#include "pch.h"

CAct65AddrOfINTERRUPT::CAct65AddrOfINTERRUPT() :CAstNode(m_pNodeTyypeName, NodeType::ADDRESS_OF_INTERRUPT)
{
}

CAct65AddrOfINTERRUPT::~CAct65AddrOfINTERRUPT()
{
}

bool CAct65AddrOfINTERRUPT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AddrOfINTERRUPT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AddrOfINTERRUPT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65AddrOfINTERRUPT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AddrOfINTERRUPT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
