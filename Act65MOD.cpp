#include "pch.h"

CAct65MOD::CAct65MOD() :CAstNode(m_pNodeTyypeName, NodeType::MOD)
{
}

CAct65MOD::~CAct65MOD()
{
}

bool CAct65MOD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65MOD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65MOD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65MOD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65MOD::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
