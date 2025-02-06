#include "pch.h"

CAct65TypeDotField::CAct65TypeDotField()
{
}

CAct65TypeDotField::~CAct65TypeDotField()
{
}

bool CAct65TypeDotField::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::TYPED_DOT_FIELD);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TypeDotField::Process()
{
	return nullptr;
}

void CAct65TypeDotField::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
