#include "pch.h"

CAct65AddressOF::CAct65AddressOF()
{
}

CAct65AddressOF::~CAct65AddressOF()
{
}

bool CAct65AddressOF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AddressOF::Process()
{
	return nullptr;
}

void CAct65AddressOF::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
