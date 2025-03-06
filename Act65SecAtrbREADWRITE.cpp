#include "pch.h"

CAct65SecAtrbREADWRITE::CAct65SecAtrbREADWRITE() :CAstNode(m_pNodeTyypeName, NodeType::SECTION_ATTRIBUTE_READ_WRITE)
{
}

CAct65SecAtrbREADWRITE::~CAct65SecAtrbREADWRITE()
{
}

bool CAct65SecAtrbREADWRITE::Create(CAstNode* pChild, CAstNode* pNext, CBin* pSym)
{
	return true;
}

CValue* CAct65SecAtrbREADWRITE::Process()
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

int CAct65SecAtrbREADWRITE::Print(int Indent, char* s, int Strlen, bool* pbNextFlag)
{
	int l = 0;

	l = CAstNode::Print(Indent, s, Strlen, pbNextFlag);
	return l;
}

void CAct65SecAtrbREADWRITE::PrintNode(FILE* pOut, int Indent, bool* pbNextFlag)
{
	CAstNode::PrintNode(pOut, Indent, pbNextFlag);
}
