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
	return nullptr;
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
