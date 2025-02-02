#include "pch.h"

CAct65CONSTANT::CAct65CONSTANT()
{
}

CAct65CONSTANT::~CAct65CONSTANT()
{
}

bool CAct65CONSTANT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::CONSTANT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65CONSTANT::Process()
{
	return nullptr;
}

void CAct65CONSTANT::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
