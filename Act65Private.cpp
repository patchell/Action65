#include "pch.h"

CAct65Private::CAct65Private() :CAstNode(m_pNodeTyypeName, NodeType::PRIVATE)
{
}

CAct65Private::~CAct65Private()
{
}

bool CAct65Private::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Private::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65Private::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Private::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Private::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
