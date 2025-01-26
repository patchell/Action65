#include "pch.h"

CAct65BREAK::CAct65BREAK()
{
}

CAct65BREAK::~CAct65BREAK()
{
}

bool CAct65BREAK::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65BREAK::Process()
{
    return nullptr;
}

void CAct65BREAK::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
