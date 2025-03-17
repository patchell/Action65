#include "pch.h"


CAct65AssignMULT::CAct65AssignMULT() :CAstNode(m_pNodeTyypeName, NodeType::ASSIGN_MULT)
{
}

CAct65AssignMULT::~CAct65AssignMULT()
{
}

bool CAct65AssignMULT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65AssignMULT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65AssignMULT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65AssignMULT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65AssignMULT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}

