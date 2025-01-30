#include "pch.h"

CAct65StatusFLAGS::CAct65StatusFLAGS()
{
}

CAct65StatusFLAGS::~CAct65StatusFLAGS()
{
}

bool CAct65StatusFLAGS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65StatusFLAGS::Process()
{
	return nullptr;
}

void CAct65StatusFLAGS::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
