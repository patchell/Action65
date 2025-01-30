#include "pch.h"

CAct65DeclareParams::CAct65DeclareParams()
{
}

CAct65DeclareParams::~CAct65DeclareParams()
{
}

bool CAct65DeclareParams::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DeclareParams::Process()
{
	return nullptr;
}

void CAct65DeclareParams::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
