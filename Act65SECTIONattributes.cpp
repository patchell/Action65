#include "pch.h"

CAct65SECTIONattributes::CAct65SECTIONattributes()
{
}

CAct65SECTIONattributes::~CAct65SECTIONattributes()
{
}

bool CAct65SECTIONattributes::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::SECTION_ATTRIBUTES);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
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
