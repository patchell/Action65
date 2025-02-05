#include "pch.h"

CAct65CharConstant::CAct65CharConstant()
{
	m_Value = ' ';
}

CAct65CharConstant::~CAct65CharConstant()
{
}

bool CAct65CharConstant::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeType(NodeType::CHAR_CONSTANT);
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65CharConstant::Process()
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

void CAct65CharConstant::Print(FILE* pOut, int Indent)
{
	if (pOut)
	{
		int l = 0;
		char* s = new char[256];
		
		l = CAstNode::Print(Indent, s, 256);
		sprintf_s(&s[l], 256, " \%c'\'", GetValue());
		fprintf(pOut, "%s\n");
		delete[] s;
	}
}
