#include "pch.h"

CAct65AssignOR::CAct65AssignOR() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_OR)
{
}

CAct65AssignOR::~CAct65AssignOR()
{
}

bool CAct65AssignOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignOR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
