#include "pch.h"

CAct65PROC::CAct65PROC() :CAstNode(m_pNodeTyypeName, NodeType::PROC)
{
}

CAct65PROC::~CAct65PROC()
{
}

bool CAct65PROC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PROC::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65PROC::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PROC::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PROC::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
