#include "pch.h"

CAct65POP::CAct65POP() :CAstNode(m_pNodeTyypeName, NodeType::POP)
{
}

CAct65POP::~CAct65POP()
{
}

bool CAct65POP::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65POP::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65POP::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65POP::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65POP::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
