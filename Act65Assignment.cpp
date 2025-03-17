#include "pch.h"

CAct65Assignment::CAct65Assignment() :CAstNode(m_pNodeTyypeName, NodeType::BASE)
{
}

CAct65Assignment::~CAct65Assignment()
{
}

bool CAct65Assignment::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65Assignment::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65Assignment::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65Assignment::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65Assignment::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
