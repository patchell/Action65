#include "pch.h"

CAct65IF::CAct65IF() :CAstNode(m_pNodeTyypeName, NodeType::IF)
{
}

CAct65IF::~CAct65IF()
{
}

bool CAct65IF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65IF::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65IF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65IF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IF::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
