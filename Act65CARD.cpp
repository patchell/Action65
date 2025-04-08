#include "pch.h"

CAct65CARD::CAct65CARD() :CAstNode(m_pNodeTyypeName, NodeType::CARD)
{
	
}

CAct65CARD::~CAct65CARD()
{
}

bool CAct65CARD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CARD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CARD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CARD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CARD::Emit(CValue* pVc, CValue* pVn)
{
//	fprintf(LogFile(), "EMIT:CARD:ID=%d\n", GetID());
	return nullptr;
}
