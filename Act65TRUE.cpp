#include "pch.h"

CAct65TRUE::CAct65TRUE() :CAstNode(m_pNodeTyypeName, NodeType::TRUE)
{
}

CAct65TRUE::~CAct65TRUE()
{
}

bool CAct65TRUE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TRUE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65TRUE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TRUE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65TRUE::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
