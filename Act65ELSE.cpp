#include "pch.h"

CAct65ELSE::CAct65ELSE() :CAstNode(m_pNodeTyypeName, NodeType::ELSE)
{
}

CAct65ELSE::~CAct65ELSE()
{
}

bool CAct65ELSE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ELSE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ELSE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ELSE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ELSE::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
