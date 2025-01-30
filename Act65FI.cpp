#include "pch.h"

CAct65FI::CAct65FI()
{
}

CAct65FI::~CAct65FI()
{
}

bool CAct65FI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FI::Process()
{
	return nullptr;
}

void CAct65FI::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
