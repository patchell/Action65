#include "pch.h"

CAct65LocalVar::CAct65LocalVar()
{
}

CAct65LocalVar::~CAct65LocalVar()
{
}

bool CAct65LocalVar::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::LOCAL_VARIABLE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65LocalVar::Process()
{
	return nullptr;
}

void CAct65LocalVar::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
