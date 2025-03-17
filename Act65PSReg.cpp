#include "pch.h"

CAct65PSReg::CAct65PSReg() :CAstNode(m_pNodeTyypeName, NodeType::PROCESSOR_STATUS_REGISTER)
{
}

CAct65PSReg::~CAct65PSReg()
{
}

bool CAct65PSReg::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PSReg::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65PSReg::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PSReg::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PSReg::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
