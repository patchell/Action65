#include "pch.h"

CAct65ConstLIST::CAct65ConstLIST()
{
}

CAct65ConstLIST::~CAct65ConstLIST()
{
}

bool CAct65ConstLIST::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65ConstLIST::Process()
{
	return nullptr;
}

void CAct65ConstLIST::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
