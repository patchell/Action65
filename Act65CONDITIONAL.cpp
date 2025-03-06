#include "pch.h"

CAct65CONDITIONAL::CAct65CONDITIONAL() : CAstNode(m_pNodeTyypeName,NodeType::CONDITIONAL)
{
}

CAct65CONDITIONAL::~CAct65CONDITIONAL()
{
}

bool CAct65CONDITIONAL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CONDITIONAL::Process()
{
	return nullptr;
}

int CAct65CONDITIONAL::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CONDITIONAL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
