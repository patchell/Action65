#include "pch.h"

CAct65OptNOT::CAct65OptNOT()
{
}

CAct65OptNOT::~CAct65OptNOT()
{
}

bool CAct65OptNOT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OPTIONAL_NOT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65OptNOT::Process()
{
	return nullptr;
}

void CAct65OptNOT::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
