#include "pch.h"

CAct65PopDest::CAct65PopDest()
{
}

CAct65PopDest::~CAct65PopDest()
{
}

bool CAct65PopDest::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PopDest::Process()
{
	return nullptr;
}

void CAct65PopDest::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
