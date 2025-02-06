#include "pch.h"

CAct65DB::CAct65DB()
{
}

CAct65DB::~CAct65DB()
{
}

bool CAct65DB::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
    return false;
}

CValue* CAct65DB::Process()
{
	return nullptr;
}

void CAct65DB::Print(FILE* pOut, int Indent)
{
	CAstNode::Print(pOut, Indent);
}
