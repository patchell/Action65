#include "pch.h"

CAct65IDENT::CAct65IDENT()
{
}

CAct65IDENT::~CAct65IDENT()
{
}

bool CAct65IDENT::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65IDENT::Process()
{
    return nullptr;
}

void CAct65IDENT::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
