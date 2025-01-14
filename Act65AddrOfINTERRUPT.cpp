#include "pch.h"

CAct65AddrOfINTERRUPT::CAct65AddrOfINTERRUPT()
{
}

CAct65AddrOfINTERRUPT::~CAct65AddrOfINTERRUPT()
{
}

bool CAct65AddrOfINTERRUPT::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65AddrOfINTERRUPT::Process()
{
	return nullptr;
}

void CAct65AddrOfINTERRUPT::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
