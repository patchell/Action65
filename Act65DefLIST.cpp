#include "pch.h"

CAct65DefLIST::CAct65DefLIST()
{
}

CAct65DefLIST::~CAct65DefLIST()
{
}

bool CAct65DefLIST::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65DefLIST::Process()
{
	return nullptr;
}

void CAct65DefLIST::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}