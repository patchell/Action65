#include "pch.h"

CAct65PointerDeREF::CAct65PointerDeREF()
{
}

CAct65PointerDeREF::~CAct65PointerDeREF()
{
}

bool CAct65PointerDeREF::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65PointerDeREF::Process()
{
	return nullptr;
}

void CAct65PointerDeREF::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
