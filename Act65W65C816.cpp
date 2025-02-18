#include "pch.h"

CAct65W65C816::CAct65W65C816() :CAstNode(NodeType::W65C816)
{
}

CAct65W65C816::~CAct65W65C816()
{
}

bool CAct65W65C816::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65W65C816::Process()
{
	return nullptr;
}

int CAct65W65C816::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65W65C816::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
