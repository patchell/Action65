#include "pch.h"

CAct65ForTO::CAct65ForTO()
{
}

CAct65ForTO::~CAct65ForTO()
{
}

bool CAct65ForTO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::FOR_TO);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ForTO::Process()
{
	return nullptr;
}

void CAct65ForTO::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
