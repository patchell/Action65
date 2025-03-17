#include "pch.h"

CAct65AssignRSH::CAct65AssignRSH() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_RSH)
{
}

CAct65AssignRSH::~CAct65AssignRSH()
{
}

bool CAct65AssignRSH::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignRSH::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignRSH::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignRSH::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignRSH::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
