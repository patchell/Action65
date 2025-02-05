#include "pch.h"

CAct65IDENT::CAct65IDENT()
{
}

CAct65IDENT::~CAct65IDENT()
{
}

bool CAct65IDENT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::IDENT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65IDENT::Process()
{
    return nullptr;
}

void CAct65IDENT::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
