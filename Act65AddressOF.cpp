#include "pch.h"

CAct65AddressOF::CAct65AddressOF() :CAstNode(NodeType::ADDRESS_OF)
{
}

CAct65AddressOF::~CAct65AddressOF()
{
}

bool CAct65AddressOF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AddressOF::Process()
{
	return nullptr;
}

int CAct65AddressOF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65AddressOF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
