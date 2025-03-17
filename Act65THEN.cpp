#include "pch.h"

CAct65THEN::CAct65THEN() :CAstNode(m_pNodeTyypeName, NodeType::THEN)
{
}

CAct65THEN::~CAct65THEN()
{
}

bool CAct65THEN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65THEN::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65THEN::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65THEN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65THEN::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
