#include "pch.h"

CAct65Statements::CAct65Statements() :CAstNode(m_pNodeTyypeName, NodeType::STATEMENTS)
{
}

CAct65Statements::~CAct65Statements()
{
}

bool CAct65Statements::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Statements::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65Statements::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Statements::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Statements::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
