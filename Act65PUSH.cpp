#include "pch.h"

CAct65PUSH::CAct65PUSH() :CAstNode(m_pNodeTyypeName, NodeType::PUSH)
{
}

CAct65PUSH::~CAct65PUSH()
{
}

bool CAct65PUSH::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PUSH::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65PUSH::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PUSH::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PUSH::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
