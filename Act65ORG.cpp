#include "pch.h"

CAct65ORG::CAct65ORG()
{
	m_Address = 0;}

CAct65ORG::~CAct65ORG()
{
}

bool CAct65ORG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::ORG);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65ORG::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
	return pValueChild;
}

void CAct65ORG::Print(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size;

		l = CAstNode::Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, "- Address:$%04X", GetAddress() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}
