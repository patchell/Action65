#include "pch.h"

CAct65RTI::CAct65RTI()
{
}

CAct65RTI::~CAct65RTI()
{
}

bool CAct65RTI::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65RTI::Process()
{
	return nullptr;
}

void CAct65RTI::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
