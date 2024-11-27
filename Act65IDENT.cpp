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
	CAstNode::CreateNode();
	return rV;
}

CValue* CAct65IDENT::Process()
{
    return nullptr;
}
