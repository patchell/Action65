#include "pch.h"

CAct65DeclareParams::CAct65DeclareParams()
{
}

CAct65DeclareParams::~CAct65DeclareParams()
{
}

bool CAct65DeclareParams::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65DeclareParams::Process()
{
	return nullptr;
}

void CAct65DeclareParams::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
