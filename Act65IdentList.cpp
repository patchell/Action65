#include "pch.h"

CAct65IdentList::CAct65IdentList()
{
}

CAct65IdentList::~CAct65IdentList()
{
}

bool CAct65IdentList::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65IdentList::Process()
{
	return nullptr;
}

void CAct65IdentList::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
