#include "pch.h"

CAct65INTERRUPT::CAct65INTERRUPT() :CAstNode(m_pNodeTyypeName, NodeType::INTERRUPT)
{
}

CAct65INTERRUPT::~CAct65INTERRUPT()
{
}

bool CAct65INTERRUPT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65INTERRUPT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65INTERRUPT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65INTERRUPT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65INTERRUPT::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
