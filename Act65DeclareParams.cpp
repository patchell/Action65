#include "pch.h"

CAct65DeclareParams::CAct65DeclareParams() :CAstNode(m_pNodeTyypeName, NodeType::DECLARE_PARAMS)
{
}

CAct65DeclareParams::~CAct65DeclareParams()
{
}

bool CAct65DeclareParams::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DeclareParams::Process()
{
	return nullptr;
}

int CAct65DeclareParams::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DeclareParams::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
