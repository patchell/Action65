#include "pch.h"

CAct65RTI::CAct65RTI()
{
}

CAct65RTI::~CAct65RTI()
{
}

bool CAct65RTI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::RTI);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65RTI::Process()
{
	return nullptr;
}

void CAct65RTI::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
