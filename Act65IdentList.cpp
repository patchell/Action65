#include "pch.h"

CAct65IdentList::CAct65IdentList() :CAstNode(m_pNodeTyypeName, NodeType::IDENT_LIST)
{
}

CAct65IdentList::~CAct65IdentList()
{
}

bool CAct65IdentList::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65IdentList::Process()
{
	return nullptr;
}

int CAct65IdentList::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65IdentList::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
