#include "pch.h"

CAct65TypeDotField::CAct65TypeDotField()
{
}

CAct65TypeDotField::~CAct65TypeDotField()
{
}

bool CAct65TypeDotField::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TypeDotField::Process()
{
	return nullptr;
}

void CAct65TypeDotField::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
