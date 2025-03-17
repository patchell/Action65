#include "pch.h"

CAct65MUL::CAct65MUL() :CAstNode(m_pNodeTyypeName, NodeType::MULTIPLY)
{
}

CAct65MUL::~CAct65MUL()
{
}

bool CAct65MUL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65MUL::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65MUL::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65MUL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65MUL::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
