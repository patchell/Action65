#include "pch.h"

CAct65ParamList::CAct65ParamList() :CAstNode(m_pNodeTyypeName, NodeType::PARAMETER_LIST)
{
}

CAct65ParamList::~CAct65ParamList()
{
}

bool CAct65ParamList::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65ParamList::Process()
{
	return nullptr;
}

int CAct65ParamList::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65ParamList::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
