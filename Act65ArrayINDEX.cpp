#include "pch.h"

CAct65ArrayINDEX::CAct65ArrayINDEX()
{
}

CAct65ArrayINDEX::~CAct65ArrayINDEX()
{
}

bool CAct65ArrayINDEX::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65ArrayINDEX::Process()
{
	return nullptr;
}

void CAct65ArrayINDEX::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
