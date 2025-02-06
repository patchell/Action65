#include "pch.h"

CAct65InitDATA::CAct65InitDATA()
{
}

CAct65InitDATA::~CAct65InitDATA()
{
}

bool CAct65InitDATA::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::INIT_DATA);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65InitDATA::Process()
{
	return nullptr;
}

void CAct65InitDATA::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
