#include "pch.h"

CAct65TypeDotField::CAct65TypeDotField() :CAstNode(m_pNodeTyypeName, NodeType::TYPED_DOT_FIELD)
{
}

CAct65TypeDotField::~CAct65TypeDotField()
{
}

bool CAct65TypeDotField::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65TypeDotField::Process()
{
	return nullptr;
}

int CAct65TypeDotField::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TypeDotField::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
