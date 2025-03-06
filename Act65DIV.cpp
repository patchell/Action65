#include "pch.h"

CAct65DIV::CAct65DIV() :CAstNode(m_pNodeTyypeName, NodeType::DIVIDE)
{
}

CAct65DIV::~CAct65DIV()
{
}

bool CAct65DIV::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DIV::Process()
{
	return nullptr;
}

int CAct65DIV::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DIV::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
