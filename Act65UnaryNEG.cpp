#include "pch.h"

CAct65UnaryNEG::CAct65UnaryNEG()
{
}

CAct65UnaryNEG::~CAct65UnaryNEG()
{
}

bool CAct65UnaryNEG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::UNARY_NEG);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65UnaryNEG::Process()
{
	return nullptr;
}

void CAct65UnaryNEG::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
