#include "pch.h"

CAct65OptNOT::CAct65OptNOT()
{
}

CAct65OptNOT::~CAct65OptNOT()
{
}

bool CAct65OptNOT::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65OptNOT::Process()
{
	return nullptr;
}

void CAct65OptNOT::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
