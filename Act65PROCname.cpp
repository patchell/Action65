#include "pch.h"

CAct65PROCname::CAct65PROCname()
{
}

CAct65PROCname::~CAct65PROCname()
{
}

bool CAct65PROCname::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65PROCname::Process()
{
	return nullptr;
}

void CAct65PROCname::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
