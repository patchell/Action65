#include "pch.h"

CAct65ROR::CAct65ROR() :CAstNode(m_pNodeTyypeName, NodeType::ROR)
{
}

CAct65ROR::~CAct65ROR()
{
}

bool CAct65ROR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ROR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ROR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ROR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ROR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
