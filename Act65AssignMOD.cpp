#include "pch.h"

CAct65AssignMOD::CAct65AssignMOD() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_MOD)
{
}

CAct65AssignMOD::~CAct65AssignMOD()
{
}

bool CAct65AssignMOD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignMOD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignMOD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignMOD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignMOD::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
