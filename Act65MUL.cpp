#include "pch.h"

CAct65MUL::CAct65MUL()
{
}

CAct65MUL::~CAct65MUL()
{
}

bool CAct65MUL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::MULTIPLY);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65MUL::Process()
{
	return nullptr;
}

void CAct65MUL::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
