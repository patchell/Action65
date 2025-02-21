#include "pch.h"

CAct65AssignEQ::CAct65AssignEQ() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_EQ)
{
}

CAct65AssignEQ::~CAct65AssignEQ()
{
}

bool CAct65AssignEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65AssignEQ::Process()
{
	return nullptr;
}

int CAct65AssignEQ::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignEQ::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
