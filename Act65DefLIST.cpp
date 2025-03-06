#include "pch.h"

CAct65DefLIST::CAct65DefLIST() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_LIST)
{
}

CAct65DefLIST::~CAct65DefLIST()
{
}

bool CAct65DefLIST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DefLIST::Process()
{
	return nullptr;
}

int CAct65DefLIST::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DefLIST::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
