#include "pch.h"

CAct65DIV::CAct65DIV()
{
}

CAct65DIV::~CAct65DIV()
{
}

bool CAct65DIV::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65DIV::Process()
{
	return nullptr;
}

void CAct65DIV::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}