#include "pch.h"

CAct65Ident::CAct65Ident()
{
}

CAct65Ident::~CAct65Ident()
{
}

bool CAct65Ident::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::IDENTIFIER);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65Ident::Process()
{
	return nullptr;
}

void CAct65Ident::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
