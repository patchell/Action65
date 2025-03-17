#include "pch.h"

CAct65IDENTaddress::CAct65IDENTaddress() :CAstNode(m_pNodeTyypeName, NodeType::IDENT_ADDRESS)
{
}

CAct65IDENTaddress::~CAct65IDENTaddress()
{
}

bool CAct65IDENTaddress::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return false;
}

CValue* CAct65IDENTaddress::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65IDENTaddress::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65IDENTaddress::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IDENTaddress::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
