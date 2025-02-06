#include "pch.h"

CAct65ProcADDR::CAct65ProcADDR()
{
}

CAct65ProcADDR::~CAct65ProcADDR()
{
}

bool CAct65ProcADDR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::PROC_ADDRESS);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ProcADDR::Process()
{
	return nullptr;
}

void CAct65ProcADDR::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
