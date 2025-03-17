#include "pch.h"

CAct65NotEquelTO::CAct65NotEquelTO() :CAstNode(m_pNodeTyypeName, NodeType::EQUAL_TO)
{
}

CAct65NotEquelTO::~CAct65NotEquelTO()
{
}

bool CAct65NotEquelTO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65NotEquelTO::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65NotEquelTO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65NotEquelTO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65NotEquelTO::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
