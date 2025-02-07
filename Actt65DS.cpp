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
    return nullptr;
}

void CAct65DS::Print(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = CAstNode::Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, " = $%04X Bytes", GetSize() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}
