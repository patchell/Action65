#include "pch.h"

CAct65TypeFIELD::CAct65TypeFIELD() : CAstNode(m_pNodeTyypeName, NodeType::TYPED_DOT_FIELD)
{
}

CAct65TypeFIELD::~CAct65TypeFIELD()
{
}

bool CAct65TypeFIELD::Create(CAstNode* pChild, CAstNode* pNex, CBin* pSym)
{
	return false;
}

CValue* CAct65TypeFIELD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65TypeFIELD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65TypeFIELD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	int x = 0;
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TypeFIELD::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
