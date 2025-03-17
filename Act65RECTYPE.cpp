#include "pch.h"

CAct65RECTYPE::CAct65RECTYPE() :CAstNode(m_pNodeTyypeName, NodeType::RECORD_TYPE)
{
}

CAct65RECTYPE::~CAct65RECTYPE()
{
}

bool CAct65RECTYPE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65RECTYPE::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65RECTYPE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65RECTYPE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65RECTYPE::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
