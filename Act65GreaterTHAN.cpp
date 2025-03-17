#include "pch.h"

CAct65GreaterTHAN::CAct65GreaterTHAN() :CAstNode(m_pNodeTyypeName, NodeType::GREATER_THAN)
{
}

CAct65GreaterTHAN::~CAct65GreaterTHAN()
{
}

bool CAct65GreaterTHAN::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65GreaterTHAN::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65GreaterTHAN::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65GreaterTHAN::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65GreaterTHAN::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
