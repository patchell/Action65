#include "pch.h"

CAct65LogicalOR::CAct65LogicalOR() :CAstNode(m_pNodeTyypeName, NodeType::LOGICAL_OR)
{
}

CAct65LogicalOR::~CAct65LogicalOR()
{
}

bool CAct65LogicalOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LogicalOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LogicalOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LogicalOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LogicalOR::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
