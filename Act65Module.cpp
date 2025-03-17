#include "pch.h"

CAct65Module::CAct65Module() :CAstNode(m_pNodeTyypeName, NodeType::MODULE)
{
}

CAct65Module::~CAct65Module()
{
}

bool CAct65Module::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Module::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65Module::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Module::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Module::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
