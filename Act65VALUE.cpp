#include "pch.h"

CAct65VALUE::CAct65VALUE()
{
	m_Value = 0;
}

CAct65VALUE::~CAct65VALUE()
{
}

bool CAct65VALUE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::VALUE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65VALUE::Process()
{
	return nullptr;
}

int CAct65VALUE::Print(int Indent, char* s, int Strlen)
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

int CAct65VALUE::PrintNode(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;

		l = Print(Indent, s, l);
		sprintf_s(&s[l], 256 - l, "-%04X", GetValue()->GetConstVal());
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
	return 0;
}
