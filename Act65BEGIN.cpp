#include "pch.h"

CAct65BEGIN::CAct65BEGIN() :CAstNode(m_pNodeTyypeName, NodeType::BEGIN)
{
}

CAct65BEGIN::~CAct65BEGIN()
{
}

bool CAct65BEGIN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BEGIN::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BEGIN::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BEGIN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BEGIN::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
