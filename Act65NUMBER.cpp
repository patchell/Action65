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

void CAct65NUMBER::Print(FILE* pOut, int Indent)
{
	int l = 0;
	int size = 0;

	if (pOut)
	{
		char* s = new char[256];
		l = CAstNode::Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, "%04x", GetValue());
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}
