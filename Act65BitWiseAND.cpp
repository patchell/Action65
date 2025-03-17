#include "pch.h"

CAct65BitWiseAND::CAct65BitWiseAND() :CAstNode(m_pNodeTyypeName, NodeType::BITWISE_AND)
{
}

CAct65BitWiseAND::~CAct65BitWiseAND()
{
}

bool CAct65BitWiseAND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BitWiseAND::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BitWiseAND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BitWiseAND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BitWiseAND::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
