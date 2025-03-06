#include "pch.h"

CAct65PROCname::CAct65PROCname() :CAstNode(m_pNodeTyypeName, NodeType::PROC_NAME)
{
}

CAct65PROCname::~CAct65PROCname()
{
}

bool CAct65PROCname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PROCname::Process()
{
	return nullptr;
}

int CAct65PROCname::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	if (GetSymbol())
	{
		if (GetSymbol()->GetName())
		{
			int size;
			char* z = GetSymbol()->GetName();
			size = Strlen - l;
			l += sprintf_s(&s[l], size, " - \'%s\'", z);
		}
	}
	return l;
}

void CAct65PROCname::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
