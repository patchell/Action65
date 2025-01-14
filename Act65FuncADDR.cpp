#include "pch.h"

CAct65FuncADDR::CAct65FuncADDR()
{
}

CAct65FuncADDR::~CAct65FuncADDR()
{
}

bool CAct65FuncADDR::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65FuncADDR::Process()
{
	return nullptr;
}

void CAct65FuncADDR::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
