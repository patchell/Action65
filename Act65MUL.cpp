#include "pch.h"

CAct65MUL::CAct65MUL()
{
}

CAct65MUL::~CAct65MUL()
{
}

bool CAct65MUL::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65MUL::Process()
{
	return nullptr;
}

void CAct65MUL::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
