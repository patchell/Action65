#include "pch.h"

CAct65SUB::CAct65SUB() :CAstNode(m_pNodeTyypeName, NodeType::SUBTRACT)
{
}

CAct65SUB::~CAct65SUB()
{
}

bool CAct65SUB::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SUB::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65SUB::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SUB::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65SUB::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
