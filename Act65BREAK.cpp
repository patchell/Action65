#include "pch.h"

CAct65BREAK::CAct65BREAK() :CAstNode(m_pNodeTyypeName, NodeType::BREAK)
{
}

CAct65BREAK::~CAct65BREAK()
{
}

bool CAct65BREAK::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BREAK::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BREAK::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BREAK::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BREAK::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
