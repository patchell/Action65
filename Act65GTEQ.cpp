#include "pch.h"

CAct65GTEQ::CAct65GTEQ():CAstNode(m_pNodeTyypeName, NodeType::GREATER_THAN_OR_EQUAL)
{
}

CAct65GTEQ::~CAct65GTEQ()
{
}

bool CAct65GTEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65GTEQ::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65GTEQ::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65GTEQ::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65GTEQ::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
