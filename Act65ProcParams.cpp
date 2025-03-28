#include "pch.h"

CAct65ProcParams::CAct65ProcParams() :CAstNode(m_pNodeTyypeName, NodeType::PROC_PARAMETERS)
{
}

CAct65ProcParams::~CAct65ProcParams()
{
}

bool CAct65ProcParams::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ProcParams::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ProcParams::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ProcParams::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ProcParams::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
