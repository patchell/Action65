#include "pch.h"

CAct65TYPE::CAct65TYPE()
{
}

CAct65TYPE::~CAct65TYPE()
{
}

bool CAct65TYPE::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65TYPE::Process()
{
	return nullptr;
}

void CAct65TYPE::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
