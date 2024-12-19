#include "pch.h"

CAct65Ident::CAct65Ident()
{
}

CAct65Ident::~CAct65Ident()
{
}

bool CAct65Ident::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65Ident::Process()
{
	return nullptr;
}

void CAct65Ident::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
