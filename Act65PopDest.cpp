#include "pch.h"

CAct65PopDest::CAct65PopDest() :CAstNode(m_pNodeTyypeName, NodeType::POP_DESTINATION)
{
}

CAct65PopDest::~CAct65PopDest()
{
}

bool CAct65PopDest::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PopDest::Process()
{
	return nullptr;
}

int CAct65PopDest::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PopDest::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
