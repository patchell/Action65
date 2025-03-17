#include "pch.h"

CAct65ACC::CAct65ACC() :CAstNode(m_pNodeTyypeName, NodeType::AREG)
{
}

CAct65ACC::~CAct65ACC()
{
}

bool CAct65ACC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ACC::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
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
