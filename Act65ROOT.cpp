#include "pch.h"

CAct65ROOT::CAct65ROOT()
{
}

CAct65ROOT::~CAct65ROOT()
{
}

bool CAct65ROOT::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65ROOT::Process()
{
	return nullptr;
}

void CAct65ROOT::Print(FILE* pOut, int Indent, char* s, int l)
{
}
