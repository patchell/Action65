#include "pch.h"

CAct65DB::CAct65DB()
{
}

CAct65DB::~CAct65DB()
{
}

bool CAct65DB::Create(CAstNode* pChild, CAstNode* pNext)
{
    return false;
}

CValue* CAct65DB::Process()
{
	return nullptr;
}

void CAct65DB::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
