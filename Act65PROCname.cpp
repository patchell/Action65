#include "pch.h"

CAct65PROCname::CAct65PROCname()
{
}

CAct65PROCname::~CAct65PROCname()
{
}

bool CAct65PROCname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::PROC_NAME);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PROCname::Process()
{
	return nullptr;
}

void CAct65PROCname::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
