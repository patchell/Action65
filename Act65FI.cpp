#include "pch.h"

CAct65FI::CAct65FI() :CAstNode(m_pNodeTyypeName, NodeType::FI)
{
}

CAct65FI::~CAct65FI()
{
}

bool CAct65FI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FI::Process()
{
	return nullptr;
}

int CAct65FI::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FI::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
