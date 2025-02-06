#include "pch.h"

CAct65W65C02::CAct65W65C02()
{
}

CAct65W65C02::~CAct65W65C02()
{
}

bool CAct65W65C02::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::W65C02);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65W65C02::Process()
{
	return nullptr;
}

void CAct65W65C02::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
