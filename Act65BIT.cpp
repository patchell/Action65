#include "pch.h"

CAct65BIT::CAct65BIT() :CAstNode(m_pNodeTyypeName, NodeType::BIT)
{
}

CAct65BIT::~CAct65BIT()
{
}

bool CAct65BIT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BIT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BIT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BIT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BIT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
