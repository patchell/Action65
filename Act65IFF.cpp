#include "pch.h"

CAct65IFF::CAct65IFF() :CAstNode(m_pNodeTyypeName, NodeType::IFF)
{
}

CAct65IFF::~CAct65IFF()
{
}

bool CAct65IFF::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65IFF::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65IFF::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65IFF::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65IFF::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
