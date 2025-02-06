#include "pch.h"

CAct65FUNC::CAct65FUNC()
{
}

CAct65FUNC::~CAct65FUNC()
{
}

bool CAct65FUNC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::FUNCTION);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FUNC::Process()
{
	return nullptr;
}

void CAct65FUNC::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
