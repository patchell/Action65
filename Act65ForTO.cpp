#include "pch.h"

CAct65ForTO::CAct65ForTO() :CAstNode(NodeType::FOR_TO)
{
}

CAct65ForTO::~CAct65ForTO()
{
}

bool CAct65ForTO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ForTO::Process()
{
	return nullptr;
}

int CAct65ForTO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ForTO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
