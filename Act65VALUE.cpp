#include "pch.h"

CAct65VALUE::CAct65VALUE()
{
}

CAct65VALUE::~CAct65VALUE()
{
}

bool CAct65VALUE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::VALUE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65VALUE::Process()
{
	return nullptr;
}

void CAct65VALUE::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
