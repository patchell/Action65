#include "pch.h"

CAct65OD::CAct65OD() :CAstNode(m_pNodeTyypeName, NodeType::OD)
{
}

CAct65OD::~CAct65OD()
{
}

bool CAct65OD::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65OD::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65OD::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65OD::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65OD::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
