#include "pch.h"

CAct65ARRAYdim::CAct65ARRAYdim() : CAstNode(m_pNodeTyypeName, NodeType::ARRAY_DIM)
{
}

CAct65ARRAYdim::~CAct65ARRAYdim()
{
}

bool CAct65ARRAYdim::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ARRAYdim::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ARRAYdim::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ARRAYdim::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ARRAYdim::Emit(CValue* pVc, CValue* pVn)
{
    return nullptr;
}
