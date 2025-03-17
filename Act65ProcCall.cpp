#include "pch.h"

CAct65ProcCall::CAct65ProcCall() :CAstNode(m_pNodeTyypeName, NodeType::PROC_CALL)
{
}

CAct65ProcCall::~CAct65ProcCall()
{
}

bool CAct65ProcCall::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ProcCall::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ProcCall::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ProcCall::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ProcCall::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
