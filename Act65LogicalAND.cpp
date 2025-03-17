#include "pch.h"

CAct65LogicalAND::CAct65LogicalAND() :CAstNode(m_pNodeTyypeName, NodeType::LOGICAL_AND)
{
}

CAct65LogicalAND::~CAct65LogicalAND()
{
}

bool CAct65LogicalAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LogicalAND::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LogicalAND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LogicalAND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LogicalAND::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
