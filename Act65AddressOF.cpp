#include "pch.h"

CAct65AddressOF::CAct65AddressOF()
{
}

CAct65AddressOF::~CAct65AddressOF()
{
}

bool CAct65AddressOF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ADDRESS_OF);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AddressOF::Process()
{
	return nullptr;
}

void CAct65AddressOF::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
