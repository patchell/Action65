#include "pch.h"

CAct65ConstLIST::CAct65ConstLIST() :CAstNode(m_pNodeTyypeName, NodeType::CONSTANT_LIST)
{
}

CAct65ConstLIST::~CAct65ConstLIST()
{
}

bool CAct65ConstLIST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ConstLIST::Process()
{
	return nullptr;
}

int CAct65ConstLIST::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ConstLIST::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
