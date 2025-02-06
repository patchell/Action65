#include "pch.h"

CAct65ConstLIST::CAct65ConstLIST()
{
}

CAct65ConstLIST::~CAct65ConstLIST()
{
}

bool CAct65ConstLIST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::CONSTANT_LIST);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ConstLIST::Process()
{
	return nullptr;
}

void CAct65ConstLIST::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
