#include "pch.h"

CAct65PointerDeREF::CAct65PointerDeREF() :CAstNode(m_pNodeTyypeName, NodeType::POINTER_DEREFERENCE)
{
}

CAct65PointerDeREF::~CAct65PointerDeREF()
{
}

bool CAct65PointerDeREF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PointerDeREF::Process()
{
	return nullptr;
}

int CAct65PointerDeREF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PointerDeREF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
