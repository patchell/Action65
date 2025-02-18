#include "pch.h"

CAct65BITS::CAct65BITS() :CAstNode(NodeType::BITS)
{
}

CAct65BITS::~CAct65BITS()
{
}

bool CAct65BITS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65BITS::Process()
{
	return nullptr;
}

int CAct65BITS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BITS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
