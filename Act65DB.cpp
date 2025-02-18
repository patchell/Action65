#include "pch.h"

CAct65DB::CAct65DB() :CAstNode(NodeType::DEFINE_BYTE)
{
}

CAct65DB::~CAct65DB()
{
}

bool CAct65DB::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DB::Process()
{
	return nullptr;
}

int CAct65DB::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DB::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
