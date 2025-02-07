#include "pch.h"

CAct65PopDest::CAct65PopDest()
{
}

CAct65PopDest::~CAct65PopDest()
{
}

bool CAct65PopDest::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::POP_DESTINATION);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65PopDest::Process()
{
	return nullptr;
}

int CAct65PopDest::Print(int Indent, char* s, int Strlen)
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

int CAct65PopDest::PrintNode(FILE* pOut, int Indent)
{
	return CAstNode::PrintNode(pOut, Indent);
}
