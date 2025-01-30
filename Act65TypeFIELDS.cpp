#include "pch.h"

CAct65TypeFIELDS::CAct65TypeFIELDS()
{
}

CAct65TypeFIELDS::~CAct65TypeFIELDS()
{
}

bool CAct65TypeFIELDS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65TypeFIELDS::Process()
{
	return nullptr;
}

void CAct65TypeFIELDS::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
