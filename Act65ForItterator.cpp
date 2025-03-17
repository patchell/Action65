#include "pch.h"

CAct65ForItterator::CAct65ForItterator() :CAstNode(m_pNodeTyypeName, NodeType::FOR_ITTERATOR)
{
}

CAct65ForItterator::~CAct65ForItterator()
{
}

bool CAct65ForItterator::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ForItterator::Process()
{
	CValue* pV = 0;

	pV = CAstNode::Process();
	return pV;
}

int CAct65ForItterator::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ForItterator::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}

CValue* CAct65ForItterator::Emit(CValue* pVc, CValue* pVn)
{
	return nullptr;
}
