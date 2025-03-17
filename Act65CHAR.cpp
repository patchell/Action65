#include "pch.h"

CAct65CHAR::CAct65CHAR() :CAstNode(m_pNodeTyypeName, NodeType::CHAR)
{
}

CAct65CHAR::~CAct65CHAR()
{
}

bool CAct65CHAR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65CHAR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CHAR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CHAR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CHAR::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
