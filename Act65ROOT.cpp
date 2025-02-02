#include "pch.h"

CAct65ROOT::CAct65ROOT()
{
}

CAct65ROOT::~CAct65ROOT()
{
}

bool CAct65ROOT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ROOT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ROOT::Process()
{
	return nullptr;
}

void CAct65ROOT::Print(FILE* pOut, int Indent, char* s, int l)
{
}
