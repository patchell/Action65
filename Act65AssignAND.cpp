#include "pch.h"

CAct65AssignAND::CAct65AssignAND() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_AND)
{
}

CAct65AssignAND::~CAct65AssignAND()
{
}

bool CAct65AssignAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignAND::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignAND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignAND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignAND::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
