#include "pch.h"

CAct65InitDATA::CAct65InitDATA()
{
}

CAct65InitDATA::~CAct65InitDATA()
{
}

bool CAct65InitDATA::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65InitDATA::Process()
{
	return nullptr;
}

void CAct65InitDATA::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
