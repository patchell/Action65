#include "pch.h"

CAct65AssignLSh::CAct65AssignLSh() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_LSH)
{
}

CAct65AssignLSh::~CAct65AssignLSh()
{
}

bool CAct65AssignLSh::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignLSh::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignLSh::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignLSh::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignLSh::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
