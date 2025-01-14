#include "pch.h"

CAct65STRING::CAct65STRING()
{
}

CAct65STRING::~CAct65STRING()
{
}

bool CAct65STRING::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65STRING::Process()
{
	return nullptr;
}

void CAct65STRING::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
