#include "pch.h"

CAct65INT::CAct65INT() :CAstNode(m_pNodeTyypeName, NodeType::INT)
{
}

CAct65INT::~CAct65INT()
{
}

bool CAct65INT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65INT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65INT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65INT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65INT::Emit(CValue* pVc, CValue* pVn)
{
	fprintf(LogFile(), "EMIT:INT:ID=%d\n", GetID());
	return nullptr;
}
