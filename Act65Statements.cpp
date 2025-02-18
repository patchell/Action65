#include "pch.h"

CAct65Statements::CAct65Statements() :CAstNode(NodeType::STATEMENTS)
{
}

CAct65Statements::~CAct65Statements()
{
}

bool CAct65Statements::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65Statements::Process()
{
	return nullptr;
}

int CAct65Statements::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Statements::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
