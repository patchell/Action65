#include "pch.h"

CAct65FUNC::CAct65FUNC() :CAstNode(m_pNodeTyypeName, NodeType::FUNCTION)
{
}

CAct65FUNC::~CAct65FUNC()
{
}

bool CAct65FUNC::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65FUNC::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65FUNC::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65FUNC::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65FUNC::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
