#include "pch.h"

CAct65InitDATA::CAct65InitDATA() :CAstNode(NodeType::INIT_DATA)
{
}

CAct65InitDATA::~CAct65InitDATA()
{
}

bool CAct65InitDATA::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65InitDATA::Process()
{
	return nullptr;
}

int CAct65InitDATA::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65InitDATA::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
