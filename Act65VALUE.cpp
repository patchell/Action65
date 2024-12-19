#include "pch.h"

CAct65VALUE::CAct65VALUE()
{
}

CAct65VALUE::~CAct65VALUE()
{
}

bool CAct65VALUE::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65VALUE::Process()
{
	return nullptr;
}

void CAct65VALUE::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
