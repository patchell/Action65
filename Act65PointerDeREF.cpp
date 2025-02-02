#include "pch.h"

CAct65PointerDeREF::CAct65PointerDeREF()
{
}

CAct65PointerDeREF::~CAct65PointerDeREF()
{
}

bool CAct65PointerDeREF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::POINTER_DEREFERENCE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PointerDeREF::Process()
{
	return nullptr;
}

void CAct65PointerDeREF::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
