#include "pch.h"

CAct65FuncADDR::CAct65FuncADDR()
{
}

CAct65FuncADDR::~CAct65FuncADDR()
{
}

bool CAct65FuncADDR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::FUNCTION_ADDRESS);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65FuncADDR::Process()
{
	return nullptr;
}

void CAct65FuncADDR::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
