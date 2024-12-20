#include "pch.h"

CAct65UnaryNEG::CAct65UnaryNEG()
{
}

CAct65UnaryNEG::~CAct65UnaryNEG()
{
}

bool CAct65UnaryNEG::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65UnaryNEG::Process()
{
	return nullptr;
}

void CAct65UnaryNEG::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
