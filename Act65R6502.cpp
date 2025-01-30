#include "pch.h"

CAct65R6502::CAct65R6502()
{
}

CAct65R6502::~CAct65R6502()
{
}

bool CAct65R6502::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65R6502::Process()
{
	return nullptr;
}

void CAct65R6502::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
