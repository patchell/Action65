#include "pch.h"

CAct65NUMBER::CAct65NUMBER()
{
	m_Value = 0;
}

CAct65NUMBER::~CAct65NUMBER()
{
}

bool CAct65NUMBER::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::NUMBER);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65NUMBER::Process()
{
	return nullptr;
}

int CAct65NUMBER::PrintNode(FILE* pOut, int Indent)
{
	int l = 0;
	int size = 0;

	if (pOut)
	{
		char* s = new char[256];
		l = Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, "%04X", GetValue());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
	return 0;
}

int CAct65NUMBER::Print(int Indent, char* s, int Strlen)
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
	size = Strlen - l;
	l += sprintf_s(&s[l], size, "$%04X", GetValue());
	return l;
}
