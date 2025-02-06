#include "pch.h"

CAct65ArrayINDEX::CAct65ArrayINDEX()
{
}

CAct65ArrayINDEX::~CAct65ArrayINDEX()
{
}

bool CAct65ArrayINDEX::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ARRAY_INDEX);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ArrayINDEX::Process()
{
	return nullptr;
}

void CAct65ArrayINDEX::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
