#include "pch.h"

CAct65PSReg::CAct65PSReg()
{
}

CAct65PSReg::~CAct65PSReg()
{
}

bool CAct65PSReg::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::PROCESSOR_STATUS_REGISTER);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PSReg::Process()
{
	return nullptr;
}

void CAct65PSReg::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
