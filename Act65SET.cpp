#include "pch.h"

CAct65SET::CAct65SET() :CAstNode(m_pNodeTyypeName, NodeType::SET)
{
}

CAct65SET::~CAct65SET()
{
}

bool CAct65SET::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SET::Process()
{
	return nullptr;
}

void CAct65SET::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
}

int CAct65SET::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}
