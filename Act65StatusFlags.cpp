#include "pch.h"

CAct65StatusFLAGS::CAct65StatusFLAGS()
{
}

CAct65StatusFLAGS::~CAct65StatusFLAGS()
{
}

bool CAct65StatusFLAGS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::STATUS_FLAGS);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65StatusFLAGS::Process()
{
	return nullptr;
}

void CAct65StatusFLAGS::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
