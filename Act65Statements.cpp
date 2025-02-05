#include "pch.h"

CAct65Statements::CAct65Statements()
{
}

CAct65Statements::~CAct65Statements()
{
}

bool CAct65Statements::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::STATEMENTS);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65Statements::Process()
{
	return nullptr;
}

void CAct65Statements::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
