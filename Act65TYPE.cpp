#include "pch.h"

CAct65TYPE::CAct65TYPE()
{
}

CAct65TYPE::~CAct65TYPE()
{
}

bool CAct65TYPE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::TYPE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TYPE::Process()
{
	return nullptr;
}

void CAct65TYPE::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
