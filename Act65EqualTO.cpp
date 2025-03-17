#include "pch.h"

CAct65EqualTO::CAct65EqualTO() :CAstNode(m_pNodeTyypeName, NodeType::EQUAL_TO)
{
}

CAct65EqualTO::~CAct65EqualTO()
{
}

bool CAct65EqualTO::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65EqualTO::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65EqualTO::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65EqualTO::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65EqualTO::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
