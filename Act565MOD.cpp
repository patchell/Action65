#include "pch.h"

CAct65MOD::CAct65MOD()
{
}

CAct65MOD::~CAct65MOD()
{
}

bool CAct65MOD::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65MOD::Process()
{
	return nullptr;
}

void CAct65MOD::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
