#include "pch.h"

CAct65YREG::CAct65YREG()
{
	m_Reg = RegType::Y;
}

CAct65YREG::~CAct65YREG()
{
}

bool CAct65YREG::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::YREG);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65YREG::Process()
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

void CAct65YREG::Print(FILE* pOut, int Indent)
{
	if (pOut)
	{
		char* s = new char[256];
		int l = 0;
		int size = 0;

		l = CAstNode::Print(Indent, s, l);
		size = 256 - l;
		sprintf_s(&s[l], size, " - \'Y\'");
		fprintf(pOut, "%s\n", s);
		delete[]s;
	}
}
