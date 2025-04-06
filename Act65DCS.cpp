#include "pch.h"

CAct65DCS::CAct65DCS() : CAstNode(m_pNodeTyypeName, NodeType::DCS)
{
}

CAct65DCS::~CAct65DCS()
{
}

bool CAct65DCS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DCS::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DCS::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DCS::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DCS::Emit(CValue* pVc, CValue* pVn)
{
	fprintf(LogFile(), "EMIT:DCS:ID=%d\n", GetID());
	return nullptr;
}
