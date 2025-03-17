#include "pch.h"

//--------------------------------------------
// AST node that represent a TYPE name, i.e.
// a user defined data object.
//--------------------------------------------

CAct65TYPEname::CAct65TYPEname() : CAstNode(m_pNodeTyypeName, NodeType::TYPE_NAME)
{
}

CAct65TYPEname::~CAct65TYPEname()
{
}

bool CAct65TYPEname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return false;
}

CValue* CAct65TYPEname::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65TYPEname::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
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

void CAct65TYPEname::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TYPEname::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
