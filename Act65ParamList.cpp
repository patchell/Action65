#include "pch.h"

CAct65ParamList::CAct65ParamList()
{
}

CAct65ParamList::~CAct65ParamList()
{
}

bool CAct65ParamList::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild,pNext);
}

CValue* CAct65ParamList::Process()
{
	return nullptr;
}

void CAct65ParamList::Print(FILE* pOut, int Indent, char* s)
{
	CAstNode::Print(pOut, Indent, s);
}}