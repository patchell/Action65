#include "pch.h"

CAct65ARRAYdim::CAct65ARRAYdim() : CAstNode(m_pNodeTyypeName, NodeType::ARRAY_DIM)
{
}

CAct65ARRAYdim::~CAct65ARRAYdim()
{
}

bool CAct65ARRAYdim::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ARRAYdim::Process()
{
	return nullptr;
}

int CAct65ARRAYdim::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ARRAYdim::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
