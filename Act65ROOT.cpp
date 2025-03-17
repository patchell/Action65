#include "pch.h"

CAct65ROOT::CAct65ROOT() :CAstNode(m_pNodeTyypeName, NodeType::ROOT)
{
}

CAct65ROOT::~CAct65ROOT()
{
}

bool CAct65ROOT::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ROOT::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ROOT::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ROOT::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ROOT::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
