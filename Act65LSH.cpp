#include "pch.h"

CAct65LSH::CAct65LSH() :CAstNode(m_pNodeTyypeName, NodeType::LSH)
{
}

CAct65LSH::~CAct65LSH()
{
}

bool CAct65LSH::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65LSH::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65LSH::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65LSH::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65LSH::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
