#include "pch.h"

CAct65AssignADD::CAct65AssignADD() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_ADD)
{
}

CAct65AssignADD::~CAct65AssignADD()
{
}

bool CAct65AssignADD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignADD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignADD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignADD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignADD::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
