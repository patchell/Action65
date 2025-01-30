#include "pch.h"

CAct65AdrOfCONST::CAct65AdrOfCONST()
{
}

CAct65AdrOfCONST::~CAct65AdrOfCONST()
{
}

bool CAct65AdrOfCONST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AdrOfCONST::Process()
{
	return nullptr;
}

void CAct65AdrOfCONST::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
