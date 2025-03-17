#include "pch.h"

CAct65AdrOfCONST::CAct65AdrOfCONST() :CAstNode(m_pNodeTyypeName, NodeType::ADDRESS_OF_CONSTANT)
{
}

CAct65AdrOfCONST::~CAct65AdrOfCONST()
{
}

bool CAct65AdrOfCONST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AdrOfCONST::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AdrOfCONST::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65AdrOfCONST::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AdrOfCONST::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
