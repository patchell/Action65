#include "pch.h"

CAct65RTI::CAct65RTI() :CAstNode(NodeType::RTI)
{
}

CAct65RTI::~CAct65RTI()
{
}

bool CAct65RTI::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65RTI::Process()
{
	return nullptr;
}

int CAct65RTI::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65RTI::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
