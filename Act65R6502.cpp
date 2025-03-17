#include "pch.h"

CAct65R6502::CAct65R6502() :CAstNode(m_pNodeTyypeName, NodeType::R6502)
{
}

CAct65R6502::~CAct65R6502()
{
}

bool CAct65R6502::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65R6502::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65R6502::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65R6502::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65R6502::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
