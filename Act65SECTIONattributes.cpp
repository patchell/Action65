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

int CAct65SECTIONattributes::PrintNode(FILE* pOut, int Indent)
{
	return 0;
}

int CAct65SECTIONattributes::Print(int Indent, char* s, int Strlen)
{
	int i = 0, l = 0;
	int Id, Child, Next;
	int size;

	Id = GetID();
	if (GetChild())
		Child = GetChild()->GetID();
	else
		Child = -1;
	if (GetNext())
		Next = GetNext()->GetID();
	else
		Next = -1;
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "%6d %6d %6d  ", Id, Child, Next);
	for (i = 0; i < Indent; ++i)
	{
		size = Strlen - l;
		l += sprintf_s(&s[l], size, "|  ");
	}
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "+- \'%s\'", GetNodeName());
	return l;
}
