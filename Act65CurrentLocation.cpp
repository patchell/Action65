#include "pch.h"

CAct65CurrentLocation::CAct65CurrentLocation()
{
}

CAct65CurrentLocation::~CAct65CurrentLocation()
{
}

bool CAct65CurrentLocation::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::CURRENT_LOCATION);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65CurrentLocation::Process()
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

void CAct65CurrentLocation::Print(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = CAstNode::Print(Indent, s, 256);
		size = 256 - l;
		sprintf_s(&s[l], size, " =$%04X", GetValue() & 0x0ffff);
		fprintf(pOut, "%s\n", s);
		delete[] s;
	}
}
