#include "pch.h"

CAct65SECTIONname::CAct65SECTIONname()
{
}

CAct65SECTIONname::~CAct65SECTIONname()
{
}

bool CAct65SECTIONname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OPCODE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65SECTIONname::Process()
{
	return nullptr;
}

void CAct65SECTIONname::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
