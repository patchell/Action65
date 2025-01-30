#include "pch.h"

CAct65Ident::CAct65Ident()
{
}

CAct65Ident::~CAct65Ident()
{
}

bool CAct65Ident::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65Ident::Process()
{
	return nullptr;
}

void CAct65Ident::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
