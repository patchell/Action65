#include "pch.h"

CAct65MOD::CAct65MOD()
{
}

CAct65MOD::~CAct65MOD()
{
}

bool CAct65MOD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::MOD);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65MOD::Process()
{
	return nullptr;
}

void CAct65MOD::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
