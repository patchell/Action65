#include "pch.h"

CAct65R6502::CAct65R6502()
{
}

CAct65R6502::~CAct65R6502()
{
}

bool CAct65R6502::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65R6502::Process()
{
	return nullptr;
}

void CAct65R6502::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
