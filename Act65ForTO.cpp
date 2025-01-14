#include "pch.h"

CAct65ForTO::CAct65ForTO()
{
}

CAct65ForTO::~CAct65ForTO()
{
}

bool CAct65ForTO::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65ForTO::Process()
{
	return nullptr;
}

void CAct65ForTO::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
