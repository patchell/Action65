#include "pch.h"

CAct65END::CAct65END() :CAstNode(m_pNodeTyypeName, NodeType::END)
{
}

CAct65END::~CAct65END()
{
}

bool CAct65END::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65END::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65END::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65END::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65END::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
