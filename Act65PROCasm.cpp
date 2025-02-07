#include "pch.h"

CAct65PROCasm::CAct65PROCasm()
{
}

CAct65PROCasm::~CAct65PROCasm()
{
}

bool CAct65PROCasm::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OPCODE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PROCasm::Process()
{
	return nullptr;
}

void CAct65PROCasm::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
