#include "pch.h"

CAct65NUMBER::CAct65NUMBER()
{
}

CAct65NUMBER::~CAct65NUMBER()
{
}

bool CAct65NUMBER::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65NUMBER::Process()
{
	return nullptr;
}

void CAct65NUMBER::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
