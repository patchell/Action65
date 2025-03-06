#include "pch.h"

CAct65SECTIONattributes::CAct65SECTIONattributes() :CAstNode(m_pNodeTyypeName, NodeType::SECTION_ATTRIBUTES)
{
}

CAct65SECTIONattributes::~CAct65SECTIONattributes()
{
}

bool CAct65SECTIONattributes::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SECTIONattributes::Process()
{
	return nullptr;
}

void CAct65SECTIONattributes::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
}

int CAct65SECTIONattributes::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}
