#include "pch.h"

CAct65CONST::CAct65CONST() :CAstNode(m_pNodeTyypeName, NodeType::CONST)
{
}

CAct65CONST::~CAct65CONST()
{
}

bool CAct65CONST::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return false;
}

CValue* CAct65CONST::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65CONST::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65CONST::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65CONST::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
