#include "pch.h"

CAct65FI::CAct65FI()
{
}

CAct65FI::~CAct65FI()
{
}

bool CAct65FI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::FI);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FI::Process()
{
	return nullptr;
}

void CAct65FI::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
