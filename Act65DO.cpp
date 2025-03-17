#include "pch.h"

CAct65DO::CAct65DO() :CAstNode(m_pNodeTyypeName, NodeType::DO)
{
}

CAct65DO::~CAct65DO()
{
}

bool CAct65DO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DO::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DO::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
