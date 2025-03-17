#include "pch.h"

CAct65POINTER::CAct65POINTER() :CAstNode(m_pNodeTyypeName, NodeType::POINTER)
{
}

CAct65POINTER::~CAct65POINTER()
{
}

bool CAct65POINTER::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65POINTER::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65POINTER::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65POINTER::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65POINTER::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
