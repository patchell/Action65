#include "pch.h"

CAct65W65C02::CAct65W65C02()
{
}

CAct65W65C02::~CAct65W65C02()
{
}

bool CAct65W65C02::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65W65C02::Process()
{
	return nullptr;
}

void CAct65W65C02::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
