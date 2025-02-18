#include "pch.h"

CAct65MOD::CAct65MOD() :CAstNode(NodeType::MOD)
{
}

CAct65MOD::~CAct65MOD()
{
}

bool CAct65MOD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65MOD::Process()
{
	return nullptr;
}

int CAct65MOD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65MOD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
