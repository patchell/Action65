#include "pch.h"

CAct65BODY::CAct65BODY() :CAstNode(m_pNodeTyypeName, NodeType::BODY)
{
}

CAct65BODY::~CAct65BODY()
{
}

bool CAct65BODY::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65BODY::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65BODY::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65BODY::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65BODY::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
