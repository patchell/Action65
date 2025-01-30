#include "pch.h"

CAct65DIV::CAct65DIV()
{
}

CAct65DIV::~CAct65DIV()
{
}

bool CAct65DIV::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DIV::Process()
{
	return nullptr;
}

void CAct65DIV::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
