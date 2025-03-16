#include "pch.h"

CAct65STRING::CAct65STRING() :CAstNode(m_pNodeTyypeName, NodeType::STRING)
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
	rsize_t l = 0;
	int i = 0;

	l = strlen(s) + 1;
	m_pString = new char[l+1];
	strncpy_s(m_pString, l, s,l);
}

bool CAct65STRING::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65STRING::Process()
{
	return nullptr;
}

int CAct65STRING::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65STRING::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	if (pOut)
	{
		char* s = new char[512];
		int l = 0;
		int size = 0;

		l = Print(Indent, s, 512, pbNextFlag);
		size = 256 - l;
		sprintf_s(&s[l], size, " = \"%s\"", GetString());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}
