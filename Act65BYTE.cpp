#include "pch.h"

CAct65BYTE::CAct65BYTE() :CAstNode(m_pNodeTyypeName, NodeType::BYTE)
{
}

CAct65BYTE::~CAct65BYTE()
{
}

bool CAct65BYTE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BYTE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BYTE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BYTE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BYTE::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
