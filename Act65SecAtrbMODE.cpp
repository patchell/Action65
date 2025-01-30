#include "pch.h"

CAct65SecAtrbMODE::CAct65SecAtrbMODE()
{
}

CAct65SecAtrbMODE::~CAct65SecAtrbMODE()
{
}

bool CAct65SecAtrbMODE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65SecAtrbMODE::Process()
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

void CAct65SecAtrbMODE::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s,l);
}
