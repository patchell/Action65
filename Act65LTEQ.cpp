#include "pch.h"

CAct65LTEQ::CAct65LTEQ() :CAstNode(m_pNodeTyypeName, NodeType::LESS_THAN_OR_EQUAL)
{
}

CAct65LTEQ::~CAct65LTEQ()
{
}

bool CAct65LTEQ::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LTEQ::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LTEQ::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LTEQ::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LTEQ::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
