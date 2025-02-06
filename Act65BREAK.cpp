#include "pch.h"

CAct65BREAK::CAct65BREAK()
{
}

CAct65BREAK::~CAct65BREAK()
{
}

bool CAct65BREAK::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::BREAK);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65BREAK::Process()
{
    return nullptr;
}

void CAct65BREAK::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
