#include "pch.h"

CAct65IDENT::CAct65IDENT()
{
}

CAct65IDENT::~CAct65IDENT()
{
}

bool CAct65IDENT::Create(CBin* pSym)
{
	bool rV = true;
	CValue* pV = new CValue;
	pV->Create(pSym);
	CAstNode::Create();
	return rV;
}

CValue* CAct65IDENT::Process()
{
    return nullptr;
}

void CAct65IDENT::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}
