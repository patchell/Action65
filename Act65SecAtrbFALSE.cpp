#include "pch.h"

CAct65SecAtrbFALSE::CAct65SecAtrbFALSE() :CAstNode(NodeType::SECTION_ATTRIBUTE_FALSE)
{
}

CAct65SecAtrbFALSE::~CAct65SecAtrbFALSE()
{
}

bool CAct65SecAtrbFALSE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return CAstNode::Create(pChild, pNext, pSym);
}

CValue* CAct65SecAtrbFALSE::Process()
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

int CAct65SecAtrbFALSE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SecAtrbFALSE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
