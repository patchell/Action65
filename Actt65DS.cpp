#include "pch.h"

CAct65DS::CAct65DS()
{
	m_pSection = 0;
	m_SizeInBytes = 0;
}

CAct65DS::~CAct65DS()
{
}

bool CAct65DS::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::OPCODE);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65DS::Process()
{
	return 0;
}

int CAct65DS::Print(int Indent, char* s, int Strlen)
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

int CAct65DS::PrintNode(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, " = $%04X Bytes", GetSize() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
	return 0;
}
