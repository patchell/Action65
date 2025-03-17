#include "pch.h"

CAct65UNTILL::CAct65UNTILL() :CAstNode(m_pNodeTyypeName, NodeType::UNTIL)
{
}

CAct65UNTILL::~CAct65UNTILL()
{
}

bool CAct65UNTILL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65UNTILL::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65UNTILL::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65UNTILL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65UNTILL::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
