#include "pch.h"

CAct65DefLIST::CAct65DefLIST()
{
}

CAct65DefLIST::~CAct65DefLIST()
{
}

bool CAct65DefLIST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::DEFINE_LIST);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DefLIST::Process()
{
	return nullptr;
}

void CAct65DefLIST::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
