#include "pch.h"

CAct65STRING::CAct65STRING()
{
	m_pString = 0;
	m_StringType = StringType::ACTION_STRING;
}

CAct65STRING::~CAct65STRING()
{
	if (m_pString)
		delete[] m_pString;
}

const char* CAct65STRING::GetStringTypeName() const
{
    return StringTypeLUT[int(m_StringType)].m_pName;
}

void CAct65STRING::SetString(const char* s)
{
	int l;

	l = int(strlen(s) + 1);
	m_pString = new char[l];
	strcpy_s(m_pString, l, s);
}

bool CAct65STRING::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::STRING);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65STRING::Process()
{
	return nullptr;
}

int CAct65STRING::Print(int Indent, char* s, int Strlen)
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

int CAct65STRING::PrintNode(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[512];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, 512);
		size = 256 - l;
		sprintf_s(&s[l], size, " = \"%s\"", GetString());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
	return 0;
}
