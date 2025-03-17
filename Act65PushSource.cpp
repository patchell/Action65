#include "pch.h"

CAct65PushSource::CAct65PushSource() :CAstNode(m_pNodeTyypeName, NodeType::PUSH_SOURCE)
{
}

CAct65PushSource::~CAct65PushSource()
{
}

bool CAct65PushSource::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PushSource::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65PushSource::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PushSource::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PushSource::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
