#include "pch.h"

CAct65BODY::CAct65BODY()
{
}

CAct65BODY::~CAct65BODY()
{
}

bool CAct65BODY::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::BODY);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65BODY::Process()
{
	return nullptr;
}

void CAct65BODY::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
