#include "pch.h"

CAct65AssignSUB::CAct65AssignSUB() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_SUB)
{
}

CAct65AssignSUB::~CAct65AssignSUB()
{
}

bool CAct65AssignSUB::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignSUB::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignSUB::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignSUB::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignSUB::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
