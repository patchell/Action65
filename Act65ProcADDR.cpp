#include "pch.h"

CAct65ProcADDR::CAct65ProcADDR() :CAstNode(m_pNodeTyypeName, NodeType::PROC_ADDRESS)
{
}

CAct65ProcADDR::~CAct65ProcADDR()
{
}

bool CAct65ProcADDR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ProcADDR::Process()
{
	return nullptr;
}

int CAct65ProcADDR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ProcADDR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
