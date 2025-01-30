#include "pch.h"

CAct65FUNC::CAct65FUNC()
{
}

CAct65FUNC::~CAct65FUNC()
{
}

bool CAct65FUNC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FUNC::Process()
{
	return nullptr;
}

void CAct65FUNC::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
