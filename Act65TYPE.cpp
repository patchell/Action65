#include "pch.h"

CAct65TYPE::CAct65TYPE()
{
}

CAct65TYPE::~CAct65TYPE()
{
}

bool CAct65TYPE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TYPE::Process()
{
	return nullptr;
}

void CAct65TYPE::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
