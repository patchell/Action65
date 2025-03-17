#include "pch.h"

CAct65tTYPEDEF::CAct65tTYPEDEF() :CAstNode(m_pNodeTyypeName, NodeType::TYPEDEF)
{
}

CAct65tTYPEDEF::~CAct65tTYPEDEF()
{
}

bool CAct65tTYPEDEF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65tTYPEDEF::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65tTYPEDEF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65tTYPEDEF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65tTYPEDEF::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
