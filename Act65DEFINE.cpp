#include "pch.h"

CAct65DEFINE::CAct65DEFINE() :CAstNode(m_pNodeTyypeName, NodeType::DEFINE)
{
}

CAct65DEFINE::~CAct65DEFINE()
{
}

bool CAct65DEFINE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65DEFINE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65DEFINE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65DEFINE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65DEFINE::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
