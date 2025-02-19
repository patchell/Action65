#include "pch.h"

CAct65TypeFieldsEND::CAct65TypeFieldsEND() : CAstNode(NodeType::TYPE_FIELDS_END)
{
}

CAct65TypeFieldsEND::~CAct65TypeFieldsEND()
{
}

bool CAct65TypeFieldsEND::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TypeFieldsEND::Process()
{
	return nullptr;
}

int CAct65TypeFieldsEND::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65TypeFieldsEND::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
