#include "pch.h"

CAct65BOOL::CAct65BOOL() :CAstNode(m_pNodeTyypeName, NodeType::BOOL)
{
}

CAct65BOOL::~CAct65BOOL()
{
}

bool CAct65BOOL::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BOOL::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BOOL::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BOOL::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BOOL::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
