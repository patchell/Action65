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

	l = strlen(s) + 1;
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

void CAct65STRING::Print(FILE* pOut, int Indent, char* s, int l)
{

	
	fprintf(pOut, "%s-\"%s\"\n",
		CAstNode::Print(Indent, s, l),
		m_pString
	);
}
