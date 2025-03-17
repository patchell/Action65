#include "pch.h"

CAct65PROCESSOR::CAct65PROCESSOR() :CAstNode(m_pNodeTyypeName, NodeType::PROCESSOR)
{
}

CAct65PROCESSOR::~CAct65PROCESSOR()
{
}

bool CAct65PROCESSOR::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65PROCESSOR::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65PROCESSOR::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65PROCESSOR::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65PROCESSOR::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
