#include "pch.h"

CAct65W65C816::CAct65W65C816()
{
}

CAct65W65C816::~CAct65W65C816()
{
}

bool CAct65W65C816::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::W65C816);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65W65C816::Process()
{
	return nullptr;
}

void CAct65W65C816::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
