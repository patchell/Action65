#include "pch.h"

CAct65SECTIONname::CAct65SECTIONname() :CAstNode(NodeType::SECTION_NAME)
{
}

CAct65SECTIONname::~CAct65SECTIONname()
{
}

bool CAct65SECTIONname::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65SECTIONname::Process()
{
	return nullptr;
}

int CAct65SECTIONname::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SECTIONname::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
