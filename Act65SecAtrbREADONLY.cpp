#include "pch.h"

CAct65SecAtrbREADONLY::CAct65SecAtrbREADONLY() :CAstNode(m_pNodeTyypeName, NodeType::SECTION_ATRRIBUTE_READ_ONLY)
{
}

CAct65SecAtrbREADONLY::~CAct65SecAtrbREADONLY()
{
}

bool CAct65SecAtrbREADONLY::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SecAtrbREADONLY::Process()
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

int CAct65SecAtrbREADONLY::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SecAtrbREADONLY::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
