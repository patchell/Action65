#include "pch.h"

CAct65DW::CAct65DW() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE_WORD)
{
}

CAct65DW::~CAct65DW()
{
}

bool CAct65DW::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DW::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DW::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DW::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DW::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
